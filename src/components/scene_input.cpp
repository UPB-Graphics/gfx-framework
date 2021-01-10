#include "components/scene_input.h"

#include <iostream>

#include "components/simple_scene.h"


egx::comp::SceneInput::SceneInput(SimpleScene *scene)
{
    this->scene = scene;
}


void egx::comp::SceneInput::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F3)
    {
        scene->ToggleGroundPlane();
    }

    if (key == GLFW_KEY_F5)
    {
        scene->ReloadShaders();
    }

    if (key == GLFW_KEY_ESCAPE)
    {
        scene->Exit();
    }
}
