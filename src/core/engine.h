#pragma once

/*
 *  Graphic Engine
 */

#include "core/window/window_object.h"


class Engine
{
 public:
    static WindowObject* Init(const WindowProperties &props);

    static WindowObject* GetWindow();

    // Get elapsed time in seconds since the application started
    static double GetElapsedTime();

    static void Exit();

 private:
    static WindowObject* window;
};
