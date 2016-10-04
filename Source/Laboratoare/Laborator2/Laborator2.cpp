#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{
}

void Laborator2::Init()
{
	// Create a mesh using custom data
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3( 1, -1,  1), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3( 1, -1, -1), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3( 1,  1,  1), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3( 1,  2, -1), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(-1,  2, -1), glm::vec3(1, 1, 0)),
		};

		vector<glm::vec3> positions;

		positions.push_back(glm::vec3(-1, -1,  1));
		positions.push_back(glm::vec3( 1, -1,  1));
		positions.push_back(glm::vec3( 1, -1, -1));
		positions.push_back(glm::vec3(-1, -1, -1));
		positions.push_back(glm::vec3(-1,  1,  1));
		positions.push_back(glm::vec3( 1,  1,  1));
		positions.push_back(glm::vec3( 1,  1, -1));
		positions.push_back(glm::vec3(-1,  1, -1));

		vector<glm::vec3> normals;
		normals.push_back(glm::vec3(0, 1, 1));
		normals.push_back(glm::vec3(1, 0, 1));
		normals.push_back(glm::vec3(0.5, 0.5, 0.5));
		normals.push_back(glm::vec3(1, 1, 1));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(1, 1, 0));

		vector<unsigned short> indices =
		{
			0, 1, 2,		2, 3, 0,
			4, 5, 6,		6, 7, 4,
			0, 1, 5,		5, 4, 0,
			3, 2, 6,		6, 7, 3,
			1, 2, 6,		6, 5, 1,
			3, 0, 4,		4, 7, 3,

			//0, 7, 1,		1, 6, 7,
		};

		meshes["cube1"] = new Mesh("generated cube 1");
		meshes["cube1"]->InitFromData(vertices, indices);

		meshes["cube2"] = new Mesh("generated cube 2");
		meshes["cube2"]->InitFromData(positions, normals, indices);
	}
}

void Laborator2::FrameStart()
{

}

void Laborator2::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(0, 0, 0, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render an object using a texturing shader
	RenderMesh(meshes["cube1"], shaders["ViewNormals"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render an object using a texturing shader
	RenderMesh(meshes["cube2"], shaders["ViewNormals"], glm::vec3(-1, 0.5f, 0), glm::vec3(0.5f));

}

void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

};

void Laborator2::OnKeyPress(int key, int mods)
{
	// add key press event

	if (key == GLFW_KEY_C) {
		camera->Log();
	}
};

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}