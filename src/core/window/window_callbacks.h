#pragma once

#include "utils/window_utils.h"


class WindowCallbacks
{
 private:
    WindowCallbacks() = delete;

 public:
    // Window events
    static void OnClose(GLFWwindow *W);
    static void OnResize(GLFWwindow *W, int width, int height);
    static void OnError(int error, const char* description);

    // KeyBoard
    static void KeyCallback(GLFWwindow *W, int key, int scancode, int action, int mods);

    // Mouse
    static void CursorMove(GLFWwindow *W, double posX, double posY);
    static void MouseClick(GLFWwindow *W, int button, int action, int mods);
    static void MouseScroll(GLFWwindow *W, double offsetX, double offsetY);
};
