#include "lab_extra/compute_shaders/compute_shaders.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace extra;


static inline GLuint NumGroupSize(int dataSize, int groupSize)
{
    return (dataSize + groupSize - 1) / groupSize;
}


static void DispatchCompute(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int workGroupSize, bool synchronize = true)
{
    glDispatchCompute(NumGroupSize(sizeX, workGroupSize), NumGroupSize(sizeY, workGroupSize), NumGroupSize(sizeZ, workGroupSize));
    if (synchronize) {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    CheckOpenGLError();
}


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


ComputeShaders::ComputeShaders()
{
}


ComputeShaders::~ComputeShaders()
{
    delete frameBuffer;
    delete texture1;
    delete texture2;
}


void ComputeShaders::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

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

    const string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::EXTRA, "compute_shaders", "shaders");

    // Create a shader program for rendering to texture
    {
        Shader *shader = new Shader("ComputeShaders");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("FullScreenPass");
        shader->AddShader(PATH_JOIN(shaderPath, "FullScreenPass.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FullScreenPass.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("ComputeShader");
        shader->AddShader(PATH_JOIN(shaderPath, "ComputeShader.CS.glsl"), GL_COMPUTE_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    auto resolution = window->GetResolution();

    frameBuffer = new FrameBuffer();
    frameBuffer->Generate(resolution.x, resolution.y, 3);

    texture1 = new Texture2D();
    texture1->Create(nullptr, resolution.x, resolution.y, 4);

    texture2 = new Texture2D();
    texture2->Create(nullptr, resolution.x, resolution.y, 4);
}


void ComputeShaders::FrameStart()
{
}


void ComputeShaders::Update(float deltaTimeSeconds)
{
    angle += 0.5f * deltaTimeSeconds;

    ClearScreen();

    {
        frameBuffer->Bind();
        DrawScene();
    }

    // Run compute shader
    {
        auto shader = shaders["ComputeShader"];
        shader->Use();

        glm::ivec2 resolution = frameBuffer->GetResolution();

        glBindImageTexture(0, frameBuffer->GetTextureID(0), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(1, texture1->GetTextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
        DispatchCompute(resolution.x, resolution.y, 1, 16, true);
    }

    // Render the scene normaly

    FrameBuffer::BindDefault();

    if (fullScreenPass)
    {
        {
            auto shader = shaders["FullScreenPass"];
            shader->Use();

            {
                int locTexture = shader->GetUniformLocation("texture_1");
                glUniform1i(locTexture, 0);
                frameBuffer->BindTexture(0, GL_TEXTURE0);
            }

            {
                int locTexture = shader->GetUniformLocation("texture_2");
                glUniform1i(locTexture, 1);
                frameBuffer->BindTexture(1, GL_TEXTURE0 + 1);
            }

            {
                int locTexture = shader->GetUniformLocation("texture_3");
                glUniform1i(locTexture, 2);
                frameBuffer->BindTexture(2, GL_TEXTURE0 + 2);
            }

            {
                int locTexture = shader->GetUniformLocation("texture_4");
                glUniform1i(locTexture, 3);
                glActiveTexture(GL_TEXTURE0 + 3);
                glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
            }

            int locTextureID = shader->GetUniformLocation("textureID");
            glUniform1i(locTextureID, textureID);

            glm::mat4 modelMatrix(1);
            RenderMesh(meshes["quad"], shader, modelMatrix);
        }
    }
}


void ComputeShaders::DrawScene()
{
    for (int i = 0; i < 16; i++)
    {
        float rotateAngle = (angle + i) * ((i % 2) * 2 - 1);
        glm::vec3 position = glm::vec3(-4 + (i % 4) * 2.5, 0, -2 + (i / 4) * 2.5);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
        modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));

        RenderMesh(meshes["bamboo"], shaders["ComputeShaders"], modelMatrix);
    }
}


void ComputeShaders::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void ComputeShaders::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void ComputeShaders::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F)
    {
        fullScreenPass = !fullScreenPass;
    }

    for (int i = 1; i < 9; i++)
    {
        if (key == GLFW_KEY_0 + i)
        {
            textureID = i - 1;
        }
    }
}


void ComputeShaders::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void ComputeShaders::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void ComputeShaders::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void ComputeShaders::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void ComputeShaders::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void ComputeShaders::OnWindowResize(int width, int height)
{
    frameBuffer->Resize(width, height, 32);
    // Treat window resize event
}
