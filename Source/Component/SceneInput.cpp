#include "SceneInput.h"
#include <iostream>

#include <include/gl.h>
#include <Core/Window/WindowObject.h>

#include "SimpleScene.h"

#include <Component/Transform/Transform.h>

using namespace std;

SceneInput::SceneInput(SimpleScene *scene)
{
	this->scene = scene;
}

void SceneInput::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_F3)
		scene->drawGroundPlane = !scene->drawGroundPlane;

	if (key == GLFW_KEY_F5)
		scene->ReloadShaders();

	if (key == GLFW_KEY_ESCAPE)
		scene->Exit();
}