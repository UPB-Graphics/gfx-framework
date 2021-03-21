#include "lab_spg/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace spg;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Laborator1::Laborator1()
{
}


Laborator1::~Laborator1()
{
}


void Laborator1::Init()
{
    nrInstances = 0;
    maxInstances = 50;
    shrink = 0;

    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for rendering to texture
    {
        Shader *shader = new Shader("Instances");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::SPG, "lab1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::SPG, "lab1", "shaders", "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::SPG, "lab1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Laborator1::FrameStart()
{
}


void Laborator1::Update(float deltaTimeSeconds)
{
    ClearScreen();

    {
        auto shader = shaders["Instances"];

        shader->Use();

        int loc_instances = shader->GetUniformLocation("instances");
        glUniform1i(loc_instances, nrInstances);

        // TODO(student): Add a shrinking parameter for scaling each
        // triangle in the geometry shader

        // Note that we only render a single mesh!
        RenderMesh(meshes["bamboo"], shaders["Instances"], glm::vec3(-3.3f, 0.5f, 0), glm::vec3(0.1f));
    }
}


void Laborator1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input with window->KeyHold()

    // TODO(student): Add events for modifying the shrinking parameter

}


void Laborator1::OnKeyPress(int key, int mods)
{
    // Add key press event

    if (key == GLFW_KEY_EQUAL)
    {
        nrInstances++;
        nrInstances %= maxInstances;
    }

    if (key == GLFW_KEY_MINUS)
    {
        nrInstances--;
        nrInstances %= maxInstances;
    }
}


void Laborator1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Laborator1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
