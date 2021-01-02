#include "lab4.h"

#include <stb/stb_image.h>

#include <vector>
#include <iostream>

using namespace std;
using namespace spg;


// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, 2, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
	camera->Update();


	std::string texturePath = PATH_JOIN(window->props.selfDirPath, RESOURCE_PATH::TEXTURES, "cube");
	std::string shaderPath = PATH_JOIN(window->props.selfDirPath, SOURCE_PATH::SPG, "lab4", "shaders");

	{
		Mesh* mesh = new Mesh("bunny");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDirPath, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
		mesh->UseMaterials(false);
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("cube");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDirPath, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		mesh->UseMaterials(false);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for rendering to texture
	{
		Shader *shader = new Shader("CubeMap");
		shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.FS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for rendering to texture
	{
		Shader *shader = new Shader("ShaderNormal");
		shader->AddShader(PATH_JOIN(shaderPath, "Normal.VS.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(shaderPath, "Normal.FS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	cubeMapTextureID = UploadCubeMapTexture(
		PATH_JOIN(texturePath, "posx.png"),
		PATH_JOIN(texturePath, "posy.png"),
		PATH_JOIN(texturePath, "posz.png"),
		PATH_JOIN(texturePath, "negx.png"),
		PATH_JOIN(texturePath, "negy.png"),
		PATH_JOIN(texturePath, "negz.png")
	);
}

void Laborator4::FrameStart()
{

}

void Laborator4::Update(float deltaTimeSeconds)
{
	ClearScreen();

	auto camera = GetSceneCamera();

	//pe tot ecranul
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the cubemap
	{
		Shader *shader = shaders["ShaderNormal"];
		shader->Use();

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
		int loc_texture = shader->GetUniformLocation("texture_cubemap");
		glUniform1i(loc_texture, 0);

		meshes["cube"]->Render();
	}

	// draw the reflextion on the mesh
	{
		Shader *shader = shaders["CubeMap"];
		shader->Use();

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.1f));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		auto cameraPosition = camera->m_transform->GetWorldPosition();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
		int loc_texture = shader->GetUniformLocation("texture_cubemap");
		glUniform1i(loc_texture, 0);

		int loc_camera = shader->GetUniformLocation("camera_position");
		glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		meshes["bunny"]->Render();
	}
}

void Laborator4::FrameEnd()
{
	DrawCoordinateSystem();
}

unsigned int Laborator4::UploadCubeMapTexture(const std::string &posx, const std::string &posy, const std::string &posz, const std::string& negx, const std::string& negy, const std::string& negz)
{
	int width, height, chn;

	unsigned char* data_posx = stbi_load(posx.c_str(), &width, &height, &chn, 0);
	unsigned char* data_posy = stbi_load(posy.c_str(), &width, &height, &chn, 0);
	unsigned char* data_posz = stbi_load(posz.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negx = stbi_load(negx.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negy = stbi_load(negy.c_str(), &width, &height, &chn, 0);
	unsigned char* data_negz = stbi_load(negz.c_str(), &width, &height, &chn, 0);

	// TODO - create OpenGL texture
	unsigned int textureID = 0;

	// TODO - bind the texture

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// TODO - load texture information for each face

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// free memory
	SAFE_FREE(data_posx);
	SAFE_FREE(data_posy);
	SAFE_FREE(data_posz);
	SAFE_FREE(data_negx);
	SAFE_FREE(data_negy);
	SAFE_FREE(data_negz);

	return textureID;
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator4::OnWindowResize(int width, int height)
{
	// treat window resize event
}
