#include <ctime>
#include <iostream>

#include "core/engine.h"
#include "components/simple_scene.h"

#if defined(WITH_LAB_EGC)
#   include "lab_egc/lab_list.h"
#endif

#if defined(WITH_LAB_SPG)
#   include "lab_spg/lab_list.h"
#endif

#if defined(WITH_LAB_EXTRA)
#   include "lab_extra/lab_list.h"
#endif


int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));

    // Create a window property structure
    WindowProperties wp;
    wp.resolution = glm::ivec2(1280, 720);
    wp.vSync = true;

    std::string binaryPath = std::string(argv[0]);
    wp.selfDirPath = binaryPath.substr(0, binaryPath.find_last_of("/\\"));

    // Init the Engine and create a new window with the defined properties
    WindowObject* window = Engine::Init(wp);

    // Create a new 3D world and start running it
    World *world = new SimpleScene();

    world->Init();
    world->Run();

    // Signals to the Engine to release the OpenGL context
    Engine::Exit();

    return 0;
}
