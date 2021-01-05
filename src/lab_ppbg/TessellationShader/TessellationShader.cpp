#include "TessellationShader.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include <Core/Managers/TextureManager.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/PPBG/blob/master/Source/Core/World.cpp

TessellationShader::TessellationShader()
{
}

TessellationShader::~TessellationShader()
{
}

void TessellationShader::Init()
{
	ToggleGroundPlane();

	{
		Mesh* mesh = new Mesh("quad");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		const std::string path = "Source/Laboratoare/TessellationShader/Shaders/";
		Shader *shader = new Shader("TessShader");
		shader->AddShader(path + "VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader(path + "FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->AddShader(path + "TessellationControl.glsl", GL_TESS_CONTROL_SHADER);
		shader->AddShader(path + "TessellationEval.glsl", GL_TESS_EVALUATION_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	tessFactorInner = 1;
	tessFactorOuter = 1;

	polygonMode = GL_LINE;
}

void TessellationShader::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void TessellationShader::Update(float deltaTimeSeconds)
{
	ClearScreen();

	{
		auto camera = GetSceneCamera();
		Transform T;
		T.SetWorldRotation(glm::vec3(90, 0, 0));

		auto shader = shaders["TessShader"];
		shader->Use();

		glLineWidth(3);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		// render an object using the specified shader and the specified position
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(T.GetModel()));

		int loc_tess_factor_inner = shader->GetUniformLocation("tess_factor_inner");
		int loc_tess_factor_outer = shader->GetUniformLocation("tess_factor_outer");

		glUniform1i(loc_tess_factor_inner, tessFactorInner);
		glUniform1i(loc_tess_factor_outer, tessFactorOuter);

		int loc_time = shader->GetUniformLocation("time");
		glUniform1f(loc_time, (float)Engine::GetElapsedTime());

		auto mesh = meshes["quad"];
		glBindVertexArray(mesh->GetBuffers()->VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_SHORT, 0);
	}
}

void TessellationShader::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/PPBG/blob/master/Source/Core/Window/InputController.h

void TessellationShader::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
};

void TessellationShader::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_0)
	{
		tessFactorInner++;
		cout << "Tess factor inner: " << tessFactorInner << endl;
	}

	if (key == GLFW_KEY_9)
	{
		if (tessFactorInner > 1) tessFactorInner--;
		cout << "Tess factor inner: " << tessFactorInner << endl;
	}

	if (key == GLFW_KEY_EQUAL)
	{
		tessFactorOuter++;
		cout << "Tess factor outer: " << tessFactorOuter << endl;
	}

	if (key == GLFW_KEY_MINUS)
	{
		if (tessFactorOuter > 1) tessFactorOuter--;
		cout << "Tess factor outer: " << tessFactorOuter << endl;
	}

	if (key == GLFW_KEY_F)
	{
		switch (polygonMode)
		{
			case GL_FILL:
				polygonMode = GL_LINE;
				break;

			case GL_LINE:
				polygonMode = GL_FILL;
				break;

			default:
				break;
		}
	}
};

void TessellationShader::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void TessellationShader::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void TessellationShader::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void TessellationShader::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void TessellationShader::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void TessellationShader::OnWindowResize(int width, int height)
{
	// treat window resize event
}
