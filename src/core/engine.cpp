#include "core/engine.h"

#include <iostream>

#include "core/managers/texture_manager.h"
#include "utils/gl_utils.h"


WindowObject* Engine::window = nullptr;


WindowObject* Engine::Init(const WindowProperties & props)
{
    /* Initialize the library */
    if (!glfwInit())
        exit(0);

    window = new WindowObject(props);

    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Serious problem
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(0);
    }

    TextureManager::Init(window->props.selfDir);

    return window;
}


WindowObject* Engine::GetWindow()
{
    return window;
}


void Engine::Exit()
{
    std::cout << "=====================================================" << std::endl;
    std::cout << "Engine closed. Exit" << std::endl;
    glfwTerminate();
}


double Engine::GetElapsedTime()
{
    return glfwGetTime();
}
