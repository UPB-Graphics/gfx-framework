#include "core/window/window_callbacks.h"

#include <iostream>

#include "core/engine.h"


void WindowCallbacks::KeyCallback(GLFWwindow *W, int key, int scanCode, int action, int mods)
{
    Engine::GetWindow()->KeyCallback(key, scanCode, action, mods);
}


void WindowCallbacks::CursorMove(GLFWwindow *W, double posX, double posY)
{
    Engine::GetWindow()->MouseMove((int)posX, (int)posY);
}


void WindowCallbacks::MouseClick(GLFWwindow *W, int button, int action, int mods)
{
    Engine::GetWindow()->MouseButtonCallback(button, action, mods);
}


void WindowCallbacks::MouseScroll(GLFWwindow * W, double offsetX, double offsetY)
{
    Engine::GetWindow()->MouseScroll(offsetX, offsetY);
}


void WindowCallbacks::OnClose(GLFWwindow * W)
{
    Engine::GetWindow()->Close();
}


void WindowCallbacks::OnResize(GLFWwindow *W, int width, int height)
{
    Engine::GetWindow()->SetSize(width, height);
}


void WindowCallbacks::OnError(int error, const char * description)
{
    std::cout << "[GLFW ERROR]\t" << error << "\t" << description << std::endl;
}
