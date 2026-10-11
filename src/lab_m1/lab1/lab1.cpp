#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

float culoare;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!

}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    position = glm::vec3(3, 0, 0);
    culoare = 0;
    meshCurent = "box";
    poz_x = poz_y = poz_z = 0;

    actiuneCurenta = 0; // 0, adica nu face nimic in momentul asta
    flagSpaceAgain = false;
    
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    {
        Mesh* mesh = new Mesh("archer");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters", "archer"), "archer.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

}


void Lab1::FrameStart()
{
    // std::cout << "Update" << std::endl;
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    // glClearColor(1, 1, 0, 1); - galben
    glClearColor(culoare, 0, 0, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["teapot"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes[meshCurent], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    RenderMesh(meshes["archer"], glm::vec3(poz_x, poz_y, poz_z), glm::vec3(0.01, 0.01, 0.01));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    float speed = 2.0f;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            poz_z -= speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            poz_z += speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            poz_x -= speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            poz_x += speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_Q)) {
            poz_y -= speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_E)) {
            poz_y += speed * deltaTime;
        }
    }

    if (actiuneCurenta == 1) {
        poz_y += 3.0f * deltaTime;

        if (poz_y >= 2.0f) { // daca am ajuns la punctul maxim (adica 2 metrii in cazul meu)
            actiuneCurenta = 2; // e clar ca dupa ce urca trebuie sa mai si coboare
        }
    }

    if (actiuneCurenta == 2) {
        poz_y -= 3.0f * deltaTime;

        if (poz_y <= 0.0f) {
            poz_y = 0.0f; // COBOARA SUB PODEA DACA NU ARE ASA CEVA

            if (flagSpaceAgain) {
                actiuneCurenta = 1;
                flagSpaceAgain = false;
            }
            else {
                actiuneCurenta = 0; // nu mai face nimic iar
            }
        }
    }

}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        culoare += 0.5;
    }

    if (key == GLFW_KEY_G) {
        culoare -= 0.5;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_B) {
        if (meshCurent == "box") {
            meshCurent = "sphere";
        }
        else if (meshCurent == "sphere") {
            meshCurent = "teapot";
        }
        else
            meshCurent = "box";
    }

    if (key == GLFW_KEY_SPACE) {
        if (actiuneCurenta == 0) {
            actiuneCurenta = 1;
        }
        else {
            flagSpaceAgain = true;
        }
    }

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
