#include "lab_extra/basic_text/basic_text.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace extra;


BasicText::BasicText()
{
}


BasicText::~BasicText()
{
}


void BasicText::Init()
{
    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Default mode for filling polygons
    polygonMode = GL_FILL;

    // Create the text renderer object
    glm::ivec2 resolution = window->GetResolution();
    textRenderer = new egx::comp::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    textRenderer->Load(PATH_JOIN(RESOURCE_PATH::FONTS, "arial.ttf"), 18);
}


void BasicText::DrawHUD()
{
    textRenderer->RenderText("Keys", 5.0f, 5.0f, 1.0f, glm::vec3(0.0, 1.0, 0.0));
    textRenderer->RenderText("'space'  : Toggle Solid/Wireframe/Points", 5.0f, 25.0f, 1.0f, glm::vec3(0.0, 1.0, 0.0));

    std::string polygonModeText = "";
    if (polygonMode == GL_LINE)
    {
        polygonModeText = "WIREFRAME";
    }
    if (polygonMode == GL_FILL)
    {
        polygonModeText = "SOLID";
    }
    if (polygonMode == GL_POINT)
    {
        polygonModeText = "POINTS";
    }

    textRenderer->RenderText("'z'  : Draw in wireframe mode (current mode " + polygonModeText + ")", 5.0f, 50.0f, 1.0f, glm::vec3(0.0, 1.0, 0.0));
    textRenderer->RenderText("'x'  : Draw in points mode (current mode " + polygonModeText + ")", 5.0f, 75.0f, 1.0f, glm::vec3(0.0, 1.0, 0.0));
    textRenderer->RenderText("'c'  : Draw in solid mode (current mode " + polygonModeText + ")", 5.0f, 100.0f, 1.0f, glm::vec3(0.0, 1.0, 0.0));
}


void BasicText::FrameStart()
{
}


void BasicText::Update(float deltaTimeSeconds)
{
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer
    glClearColor(0, 0, 0, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));
}


void BasicText::FrameEnd()
{
    DrawHUD();
    DrawCoordinateSystem();
}


// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h


void BasicText::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void BasicText::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_Z)
    {
        polygonMode = GL_LINE;
    }

    if (key == GLFW_KEY_X)
    {
        polygonMode = GL_POINT;
    }

    if (key == GLFW_KEY_C)
    {
        polygonMode = GL_FILL;
    }
}


void BasicText::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void BasicText::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void BasicText::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void BasicText::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void BasicText::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void BasicText::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
