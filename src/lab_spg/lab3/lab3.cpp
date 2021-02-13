#include "lab_spg/lab3/lab3.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace spg;


static float angle = 0;


// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/World.cpp


Laborator3::Laborator3()
{
}


Laborator3::~Laborator3()
{
}


void Laborator3::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for rendering to texture
    {
        Shader *shader = new Shader("ShaderLab3");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::SPG, "lab3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::SPG, "lab3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    auto resolution = window->GetResolution();

    // TODO(student): Create a new framebuffer and generate attached textures

}


void Laborator3::FrameStart()
{
}


void Laborator3::Update(float deltaTimeSeconds)
{
    static glm::vec3 mirrorPos(0, 4, -6);
    static glm::vec3 mirrorRotation = glm::vec3(0, RADIANS(180), 0);

    angle += 0.5f * deltaTimeSeconds;

    ClearScreen();

    // Save camera position and rotation
    auto camera = GetSceneCamera();

    glm::vec3 camPosition = camera->m_transform->GetWorldPosition();
    glm::quat camRotation = camera->m_transform->GetWorldRotation();

    // TODO(student): Render scene view from a mirrorred point of view. Use
    // `camera->SetPosition()` and `camera->SetRotation(glm::quat(euler_angles))`
    {

    }

    // Render the scene normally
    {
        camera->SetPosition(camPosition);
        camera->SetRotation(camRotation);

        DrawScene();
    }

    // Render the mirrored scene
    {
        auto shader = shaders["ShaderLab3"];
        
        // TODO(student): Use the mirror texture

        glm::mat4 modelMatrix(1);
        modelMatrix = glm::translate(modelMatrix, mirrorPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        RenderMesh(meshes["quad"], shader, modelMatrix);
    }
}


void Laborator3::DrawScene()
{
    for (int i = 0; i < 16; i++)
    {
        float rotateAngle = (angle + i) * ((i % 2) * 2 - 1);
        glm::vec3 position = glm::vec3(-4 + (i % 4) * 2.5, 0, -2 + (i / 4) * 2.5);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
        modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));

        RenderMesh(meshes["bamboo"], shaders["ShaderLab3"], modelMatrix);
    }
}


void Laborator3::FrameEnd()
{
    DrawCoordinateSystem();
}


// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/SPG-Framework/blob/master/Source/Core/Window/InputController.h


void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Laborator3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Laborator3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Laborator3::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
