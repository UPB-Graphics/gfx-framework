#include "components/camera_input.h"

#include <iostream>

#include "core/engine.h"
#include "utils/memory_utils.h"


gfxc::CameraInput::CameraInput(Camera *camera)
{
    this->camera = camera;
}


void gfxc::CameraInput::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) return;

    if (window->GetSpecialKeyState() & GLFW_MOD_SHIFT)
    {
        deltaTime *= 2;
    }

    if (window->KeyHold(GLFW_KEY_W))            camera->MoveForward(deltaTime);
    if (window->KeyHold(GLFW_KEY_S))            camera->MoveBackward(deltaTime);
    if (window->KeyHold(GLFW_KEY_A))            camera->MoveLeft(deltaTime);
    if (window->KeyHold(GLFW_KEY_D))            camera->MoveRight(deltaTime);
    if (window->KeyHold(GLFW_KEY_Q))            camera->MoveDown(deltaTime);
    if (window->KeyHold(GLFW_KEY_E))            camera->MoveUp(deltaTime);

    if (window->KeyHold(GLFW_KEY_KP_MULTIPLY))  camera->UpdateSpeed();
    if (window->KeyHold(GLFW_KEY_KP_DIVIDE))    camera->UpdateSpeed(-0.2f);

    if (window->KeyHold(GLFW_KEY_KP_4))         camera->RotateOY( 500 * deltaTime);
    if (window->KeyHold(GLFW_KEY_KP_6))         camera->RotateOY(-500 * deltaTime);
    if (window->KeyHold(GLFW_KEY_KP_8))         camera->RotateOX( 700 * deltaTime);
    if (window->KeyHold(GLFW_KEY_KP_5))         camera->RotateOX(-700 * deltaTime);

    camera->Update();
}


void gfxc::CameraInput::OnKeyPress(int key, int mods) {
    if (mods)
    {
        return;
    }

    if (key == GLFW_KEY_C)
    {
        camera->Log();
    }
}


void gfxc::CameraInput::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        camera->RotateOY(-(float)deltaX);
        camera->RotateOX(-(float)deltaY);
        camera->Update();
    }
}


void gfxc::CameraInput::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT))
    {
        window->DisablePointer();
    }
}


void gfxc::CameraInput::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT))
    {
        window->ShowPointer();
    }
}
