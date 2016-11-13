#include "SimpleScene.h"

#include <vector>
#include <iostream>

#include "CameraInput.h"
#include "SceneInput.h"

#include <Core/Engine.h>
#include <Component/Transform/Transform.h>

using namespace std;
using namespace EngineComponents;

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

	cameraInput = new CameraInput(camera);
	window = Engine::GetWindow();

	SceneInput *SI = new SceneInput(this);

	xozPlane = new Mesh("plane");
	xozPlane->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");

	{
		vector<VertexFormat> vertices =
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
		};
		std::vector<unsigned short> indices = { 0, 1 };

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

void SimpleScene::AddMeshToList(Mesh * mesh)
{
	if (mesh->GetMeshID())
	{
		meshes[mesh->GetMeshID()] = mesh;
	}
}

void SimpleScene::DrawCoordinatSystem()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), camera->GetProjectionMatrix());
}

void SimpleScene::DrawCoordinatSystem(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMaxtix)
{
	glLineWidth(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render the coordinate system
	{
		Shader *shader = shaders["Color"];
		shader->Use();
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMaxtix));

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
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

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

void SimpleScene::RenderMesh2D(Mesh * mesh, Shader * shader, const glm::mat3 &modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	
	glm::mat3 mm = modelMatrix;
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
	mesh->Render();
}

void SimpleScene::RenderMesh2D(Mesh * mesh, const glm::mat3 & modelMatrix, const glm::vec3 & color) const
{
	Shader* shader = shaders.at("Color");

	if (!mesh || !shader || !shader->program)
		return;

	glm::mat3 mm = modelMatrix;
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(shader->GetUniformLocation("color"), color.r, color.g, color.b);

	mesh->Render();
}

void SimpleScene::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
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

Camera * SimpleScene::GetSceneCamera() const
{
	return camera;
}

InputController * SimpleScene::GetCameraInput() const
{
	return cameraInput;
}

