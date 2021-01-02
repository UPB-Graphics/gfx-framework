#include "lab2.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace spg;


// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{
}

void Laborator2::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::quat(glm::vec3(-40 * TO_RADIANS, 0, 0)));
	camera->Update();

	ToggleGroundPlane();

	// Create a shader program for surface generation
	{
		Shader *shader = new Shader("SurfaceGeneration");
		shader->AddShader(PATH_JOIN(window->props.selfDirPath, SOURCE_PATH::SPG, "lab2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDirPath, SOURCE_PATH::SPG, "lab2", "shaders", "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDirPath, SOURCE_PATH::SPG, "lab2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//parameters related to surface generation
	no_of_generated_points = 10;	//number of points on a Bezier curve
	no_of_instances = 5;			//number of instances (number of curves that contain the surface)
	max_translate = 8.0f;			//for the translation surface, it's the distance between the first and the last curve
	max_rotate = glm::radians(360.0f);	//for the rotation surface, it's the angle between the first and the last curve

	//define control points
	control_p1 = glm::vec3(-4, -2.5, 1.0);
	control_p2 = glm::vec3(-2.5, 1.5, 1.0);
	control_p3 = glm::vec3(-1.5, 3.0, 1.0);
	control_p4 = glm::vec3(-4.0, 5.5, 1.0);


	// Create a bogus mesh with 2 points (a line)
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-4.0, -2.5,  1.0), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3(-4.0, 5.5,  1.0), glm::vec3(0, 1, 0))
		};

		vector<unsigned short> indices =
		{
			0, 1
		};

		meshes["surface"] = new Mesh("generated initial surface points");
		meshes["surface"]->InitFromData(vertices, indices);
		meshes["surface"]->SetDrawMode(GL_LINES);
	}
}


void Laborator2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator2::RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader 
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object instanced
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0,instances);

}


void Laborator2::Update(float deltaTimeSeconds)
{
	ClearScreen();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader *shader = shaders["SurfaceGeneration"];
	shader->Use();

	//send uniforms to shaders
	glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
	glUniform3f(glGetUniformLocation(shader->program, "control_p4"), control_p4.x, control_p4.y, control_p4.z);
	glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), no_of_instances);

	//TODO 
	//trimitei la shadere numarul de puncte care aproximeaza o curba (no_of_generated_points)
	//si caracteristici pentru crearea suprafetelor de translatie/rotatie (max_translate, max_rotate)
	
	Mesh* mesh = meshes["surface"];
	//draw the object instanced
	RenderMeshInstanced(mesh, shader, glm::mat4(1), no_of_instances);
	
}

void Laborator2::FrameEnd()
{
	//DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
}

void Laborator2::OnKeyPress(int key, int mods)
{
	
	//TODO 
	//modificati numarul de instante si numarul de puncte generate
}

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator2::OnWindowResize(int width, int height)
{
	// treat window resize event
}
