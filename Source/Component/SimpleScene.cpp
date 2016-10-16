#include "SimpleScene.h"

#include <vector>
#include <iostream>

#include "CameraInput.h"
#include "SceneInput.h"

#include <Core/Engine.h>
#include <Component/Transform/Transform.h>

using namespace std;

SimpleScene::SimpleScene()
{
	InitResources();
}

SimpleScene::~SimpleScene()
{
}

void SimpleScene::InitResources()
{
	// sets common GL states
	glClearColor(0, 0, 0, 1);

	drawGroundPlane = true;

	objectModel = new Transform();

	camera = new Camera();
	camera->SetPerspective(60, window->props.aspectRatio, 0.01f, 200);
	camera->transform->SetMoveSpeed(2);
	camera->transform->SetWorldPosition(glm::vec3(0, 1.6f, 2.5));
	camera->transform->SetWorldRotation(glm::vec3(-15, 0, 0));
	camera->Update();

	CameraInput *CI = new CameraInput(camera);
	SceneInput *SI = new SceneInput(this);

	xozPlane = new Mesh("plane");
	xozPlane->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");

	{
		vector<VertexFormat> vertices =
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
		};
		std::vector<unsigned short> indices = { 0, 1, 2, 1, 3, 2 };

		simpleLine = new Mesh("line");
		simpleLine->InitFromData(vertices, indices);
		simpleLine->SetDrawMode(GL_LINES);
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("Simple");
		shader->AddShader(RESOURCE_PATH::SHADERS + "MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "Default.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing vertex colors
	{
		Shader *shader = new Shader("Color");
		shader->AddShader(RESOURCE_PATH::SHADERS + "MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "Color.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("VertexNormal");
		shader->AddShader(RESOURCE_PATH::SHADERS + "MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "Normals.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Create a shader program for drawing vertex colors
	{
		Shader *shader = new Shader("VertexColor");
		shader->AddShader(RESOURCE_PATH::SHADERS + "MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "VertexColor.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Default rendering mode will use depth buffer
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void SimpleScene::DrawCoordinatSystem()
{
	glLineWidth(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render the coordinate system
	{
		Shader *shader = shaders["Color"];
		shader->Use();
		glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(camera->GetProjectionMatrix()));

		if (drawGroundPlane)
		{
			objectModel->SetScale(glm::vec3(1));
			objectModel->SetWorldPosition(glm::vec3(0));
			glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
			glUniform3f(shader->GetUniformLocation("color"), 0.5f, 0.5f, 0.5f);
			xozPlane->Render();
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glLineWidth(3);
		objectModel->SetScale(glm::vec3(1, 25, 1));
		objectModel->SetWorldRotation(glm::quat());
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
		glUniform3f(shader->GetUniformLocation("color"), 0, 1, 0);
		simpleLine->Render();

		objectModel->SetWorldRotation(glm::vec3(0, 0, -90));
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
		glUniform3f(shader->GetUniformLocation("color"), 1, 0, 0);
		simpleLine->Render();

		objectModel->SetWorldRotation(glm::vec3(90, 0, 0));
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
		glUniform3f(shader->GetUniformLocation("color"), 0, 0, 1);
		simpleLine->Render();

		objectModel->SetWorldRotation(glm::quat());

		glLineWidth(1);
	}
}

void SimpleScene::RenderMesh(Mesh * mesh, Shader * shader, glm::vec3 position, glm::vec3 scale)
{
	if (!mesh || !shader)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(camera->GetProjectionMatrix()));

	glm::mat4 model(1);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
	mesh->Render();
}

void SimpleScene::RenderMesh(Mesh * mesh, glm::vec3 position, glm::vec3 scale)
{
	RenderMesh(mesh, shaders["Simple"], position, scale);
}

void SimpleScene::ReloadShaders() const
{
	cout << endl;
	cout << "=============================" << endl;
	cout << "Reloading Shaders" << endl;
	cout << "=============================" << endl;
	cout << endl;

	for (auto &shader : shaders)
	{
		shader.second->Reload();
	}
}

