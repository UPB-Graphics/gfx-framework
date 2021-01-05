#include "ComputeShadersEx.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

static inline GLuint NumGroupSize(int dataSize, int groupSize)
{
	return (dataSize + groupSize - 1) / groupSize;
}

static void DispatchCompute(uint sizeX, uint sizeY, uint sizeZ, uint workGroupSize, bool synchronize = true)
{
	glDispatchCompute(NumGroupSize(sizeX, workGroupSize), NumGroupSize(sizeY, workGroupSize), NumGroupSize(sizeZ, workGroupSize));
	if (synchronize) {
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
	CheckOpenGLError();
}

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

ComputeShadersEx::ComputeShadersEx()
{
}

ComputeShadersEx::~ComputeShadersEx()
{
}

void ComputeShadersEx::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
	camera->Update();

	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo", "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("quad");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
		mesh->UseMaterials(false);
		meshes[mesh->GetMeshID()] = mesh;
	}

	std::string shaderPath = "Source/Laboratoare/ComputeShadersEx/Shaders/";

	// Create a shader program for rendering to texture
	{
		Shader *shader = new Shader("ComputeShadersEx");
		shader->AddShader(shaderPath + "VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader *shader = new Shader("FullScreenPass");
		shader->AddShader(shaderPath + "FullScreenPass.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "FullScreenPass.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader *shader = new Shader("Blur");
		shader->AddShader(shaderPath + "Blur.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(shaderPath + "Blur.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader *shader = new Shader("ComputeShader");
		shader->AddShader(shaderPath + "ComputeShader.CS.glsl", GL_COMPUTE_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	auto resolution = window->GetResolution();

	frameBuffer = new FrameBuffer();
	frameBuffer->Generate(resolution.x, resolution.y, 3);

	frameBufferBlur = new FrameBuffer();
	frameBufferBlur->Generate(resolution.x, resolution.y, 1, false, 8);

	textureBlur = new Texture2D();
	textureBlur->Create(nullptr, resolution.x, resolution.y, 4);
}

void ComputeShadersEx::FrameStart()
{

}

void ComputeShadersEx::Update(float deltaTimeSeconds)
{
	angle += 0.5f * deltaTimeSeconds;

	ClearScreen();

	{
		frameBuffer->Bind();
		DrawScene();
	}

	glFinish();

	const int nrTimers = 3;
	GLuint64 timers[nrTimers];
	unsigned int queryID[nrTimers];
	glGenQueries(nrTimers, queryID);
	
	glQueryCounter(queryID[0], GL_TIMESTAMP);

	{
		auto tStart = std::chrono::high_resolution_clock::now();

		// Blur using a framebuffer
		{
			auto shader = shaders["Blur"];
			shader->Use();

			frameBufferBlur->Bind();

			{
				int locTexture = shader->GetUniformLocation("texture_1");
				glUniform1i(locTexture, 0);
				frameBuffer->BindTexture(0, GL_TEXTURE0);
			}

			glm::mat4 modelMatrix(1);
			RenderMesh(meshes["quad"], shader, modelMatrix);
		}

		//glFinish();

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count();
		//cout << "FB_Time: " << diff << " ms" << endl;
	}

	glQueryCounter(queryID[1], GL_TIMESTAMP);

	
	{
		auto tStart = std::chrono::high_resolution_clock::now();

		// Run compute shader
		{
			auto shader = shaders["ComputeShader"];
			shader->Use();

			glm::ivec2 resolution = frameBuffer->GetResolution();

			glBindImageTexture(0, frameBuffer->GetTextureID(0), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			glBindImageTexture(1, textureBlur->GetTextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
			DispatchCompute(resolution.x, resolution.y, 1, 16, true);
		}

		//glFinish();

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count();
		//cout << "CS_Time: " << diff << " ms" << endl;
	}

	glQueryCounter(queryID[2], GL_TIMESTAMP);
	glFinish();

	for (int i = 0; i < nrTimers; i++)
	{
		glGetQueryObjectui64v(queryID[i], GL_QUERY_RESULT, &timers[i]);
	}

	printf("Time spent on the GPU FB: %f ms\n", (timers[1] - timers[0]) / 1000000.0);
	printf("Time spent on the GPU CB: %f ms\n", (timers[2] - timers[1]) / 1000000.0);

	// Render the scene normaly

	FrameBuffer::BindDefault();

	if (fullScreenPass)
	{
		{
			auto shader = shaders["FullScreenPass"];
			shader->Use();

			{
				int locTexture = shader->GetUniformLocation("texture_1");
				glUniform1i(locTexture, 0);
				frameBuffer->BindTexture(0, GL_TEXTURE0);
			}

			{
				int locTexture = shader->GetUniformLocation("texture_2");
				glUniform1i(locTexture, 1);
				frameBuffer->BindTexture(1, GL_TEXTURE0 + 1);
			}

			{
				int locTexture = shader->GetUniformLocation("texture_3");
				glUniform1i(locTexture, 2);
				frameBuffer->BindTexture(2, GL_TEXTURE0 + 2);
			}

			{
				int locTexture = shader->GetUniformLocation("texture_4");
				glUniform1i(locTexture, 3);
				glActiveTexture(GL_TEXTURE0 + 3);
				glBindTexture(GL_TEXTURE_2D, textureBlur->GetTextureID());
			}

			{
				int locTexture = shader->GetUniformLocation("texture_5");
				glUniform1i(locTexture, 4);
				glActiveTexture(GL_TEXTURE0 + 4);
				glBindTexture(GL_TEXTURE_2D, frameBufferBlur->GetTextureID(0));
			}

			int locTextureID = shader->GetUniformLocation("textureID");
			glUniform1i(locTextureID, textureID);

			glm::mat4 modelMatrix(1);
			RenderMesh(meshes["quad"], shader, modelMatrix);
		}
	}
}

void ComputeShadersEx::DrawScene()
{
	for (int i = 0; i < 16; i++)
	{
		float rotateAngle = (angle + i) * ((i % 2) * 2 - 1);
		glm::vec3 position = glm::vec3(-4 + (i % 4) * 2.5, 0, -2 + (i / 4) * 2.5);

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
		modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));

		RenderMesh(meshes["bamboo"], shaders["ComputeShadersEx"], modelMatrix);
	}
}

void ComputeShadersEx::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void ComputeShadersEx::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
};

void ComputeShadersEx::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F)
	{
		fullScreenPass = !fullScreenPass;
	}

	for (int i = 1; i < 9; i++)
	{
		if (key == GLFW_KEY_0 + i)
		{
			textureID = i - 1;
		}
	}
};

void ComputeShadersEx::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void ComputeShadersEx::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void ComputeShadersEx::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void ComputeShadersEx::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void ComputeShadersEx::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void ComputeShadersEx::OnWindowResize(int width, int height)
{
	frameBuffer->Resize(width, height, 32);
	// treat window resize event
}
