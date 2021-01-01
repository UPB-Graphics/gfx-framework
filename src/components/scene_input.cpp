#include "scene_input.h"

#include "simple_scene.h"

#include "include/gl.h"
#include "core/window/window_object.h"
#include "components/transform.h"

#include <iostream>

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
