#include "lab_extra/basic_text/basic_text.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace extra;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


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
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
}


void BasicText::DrawHUD()
{
    const float kTopY = 10.f;
    const float kRowHeight = 25.f;

    int rowIndex = 0;
    std::string polygonModeText = "";

    if (polygonMode == GL_LINE)
    {
        polygonModeText = "wireframe";
    }
    if (polygonMode == GL_FILL)
    {
        polygonModeText = "solid";
    }
    if (polygonMode == GL_POINT)
    {
        polygonModeText = "points";
    }

    textRenderer->RenderText("Keys",                                    5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    textRenderer->RenderText("space : toggle solid/wireframe/points",   5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    textRenderer->RenderText("    z : draw in wireframe mode",          5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    textRenderer->RenderText("    x : draw in points mode",             5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    textRenderer->RenderText("    c : draw in solid mode",              5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    
    textRenderer->RenderText("Current mode: " + polygonModeText,        5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
}


void BasicText::FrameStart()
{
}


void BasicText::Update(float deltaTimeSeconds)
{
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer
    glClearColor(kBackgroundColor.r, kBackgroundColor.g, kBackgroundColor.b, 1);

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


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


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
