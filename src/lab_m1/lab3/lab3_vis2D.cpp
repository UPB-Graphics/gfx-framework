#include "lab_m1/lab3/lab3_vis2D.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3_Vis2D::Lab3_Vis2D()
{
}


Lab3_Vis2D::~Lab3_Vis2D()
{
}


void Lab3_Vis2D::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    Mesh* square1 = object2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0));
    AddMeshToList(square1);
}


// 2D visualization matrix
glm::mat3 Lab3_Vis2D::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Lab3_Vis2D::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Lab3_Vis2D::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Lab3_Vis2D::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Lab3_Vis2D::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window
    viewSpace = ViewportSpace(0, 0, resolution.x / 2, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

    DrawScene(visMatrix);

    // The viewport is now the right half of the window

    viewSpace = ViewportSpace(resolution.x / 2, 0, resolution.x / 2, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix);
}


void Lab3_Vis2D::FrameEnd()
{
}


void Lab3_Vis2D::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(3, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(1.5, 1.5);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(0, 3);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(3, 3);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)

    // TODO(student): Zoom in and zoom out logic window with Z and X

}


void Lab3_Vis2D::OnKeyPress(int key, int mods)
{
}


void Lab3_Vis2D::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
