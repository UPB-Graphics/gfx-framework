#include "lab_m2/lab6/lab6.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;


#define rand01 (rand() / static_cast<float>(RAND_MAX))


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab6::Lab6()
{
}


Lab6::~Lab6()
{
}


void Lab6::Init()
{
    outputType = 0;

    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 2, 3.5), glm::quat(glm::vec3(-20 * TO_RADIANS, 0, 0)));
    camera->Update();

    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "ground.jpg");

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    LoadShader("Render2Texture");
    LoadShader("Composition");
    LoadShader("LightPass");

    auto resolution = window->GetResolution();

    frameBuffer = new FrameBuffer();
    frameBuffer->Generate(resolution.x, resolution.y, 3);

    lightBuffer = new FrameBuffer();
    lightBuffer->Generate(resolution.x, resolution.y, 1);

    int gridSize = 3;
    for (int i = -gridSize; i < gridSize; i++)
    {
        for (int j = -gridSize; j < gridSize; j++)
        {
            LightInfo L;
            L.color = glm::vec3(rand01, rand01, rand01);
            L.position = glm::vec3(i, rand01, j) * glm::vec3(3, 2, 3) + glm::vec3(0, 0.5, 0);
            L.radius = rand01 + 3;
            lights.push_back(L);
        }
    }
}


void Lab6::FrameStart()
{
}


void Lab6::Update(float deltaTimeSeconds)
{
    ClearScreen();

    // ------------------------------------------------------------------------
    // Deferred rendering pass
    {
        frameBuffer->Bind();

        auto shader = shaders["Render2Texture"];

        TextureManager::GetTexture("default.png")->BindToTextureUnit(GL_TEXTURE0);

        // Render scene objects
        RenderMesh(meshes["box"], shader, glm::vec3(1.5, 0.5f, 0), glm::vec3(0.5f));
        RenderMesh(meshes["box"], shader, glm::vec3(0, 1.05f, 0), glm::vec3(2));
        RenderMesh(meshes["box"], shader, glm::vec3(-2, 1.5f, 0));
        RenderMesh(meshes["sphere"], shader, glm::vec3(-4, 1, 1));

        TextureManager::GetTexture("ground.jpg")->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["plane"], shader, glm::vec3(0, 0, 0), glm::vec3(0.5f));

        // Render a simple point light bulb for each light (for debugging purposes)
        for (auto &l : lights)
        {
            auto model = glm::translate(glm::mat4(1), l.position);
            model = glm::scale(model, glm::vec3(0.2f));
            RenderMesh(meshes["sphere"], shader, model);
        }
    }

    // ------------------------------------------------------------------------
    // Lighting pass
    {
        lightBuffer->Bind();

        // Enable buffer color accumulation
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        auto shader = shaders["LightPass"];
        shader->Use();

        {
            int texturePositionsLoc = shader->GetUniformLocation("texture_position");
            glUniform1i(texturePositionsLoc, 0);
            frameBuffer->BindTexture(0, GL_TEXTURE0);
        }

        {
            int textureNormalsLoc = shader->GetUniformLocation("texture_normal");
            glUniform1i(textureNormalsLoc, 1);
            frameBuffer->BindTexture(1, GL_TEXTURE0 + 1);
        }

        auto camera = GetSceneCamera();
        glm::vec3 cameraPos = camera->m_transform->GetWorldPosition();
        int loc_eyePosition = shader->GetUniformLocation("eye_position");
        glUniform3fv(loc_eyePosition, 1, glm::value_ptr(cameraPos));

        auto resolution = window->GetResolution();
        int loc_resolution = shader->GetUniformLocation("resolution");
        glUniform2i(loc_resolution, resolution.x, resolution.y);

        // TODO(student): This section controls face culling for the light
        // areas of the spheres. Change the code so that you can switch
        // programatically between no culling, front culling and back culling.
        // This will allow you to observe the differences much more easily.
        //
        // When GL_BACK is culled, the light area of a sphere will disappear
        // if the camera enters that light sphere. This happens because the
        // inside of the sphere is not rendered.
        //
        // When GL_FRONT is culled, the light area will always be visible.
        // This is the desired effect.
        //
        // If no culling is active (meaning that both GL_FRONT and GL_BACK
        // faces are rendered), then the light area will double the intensity
        // for each pixel.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        for (auto &l : lights)
        {
            auto model = glm::translate(glm::mat4(1), l.position);
            model = glm::scale(model, glm::vec3(2 * l.radius));
            glUniform1f(shader->loc_light_radius, l.radius);
            glUniform3fv(shader->loc_light_color, 1, glm::value_ptr(l.color));
            glUniform3fv(shader->loc_light_pos, 1, glm::value_ptr(l.position));
            RenderMesh(meshes["sphere"], shader, model);
        }

        glDisable(GL_CULL_FACE);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    // ------------------------------------------------------------------------
    // Composition pass
    {
        FrameBuffer::BindDefault();

        auto shader = shaders["Composition"];
        shader->Use();

        int outputTypeLoc = shader->GetUniformLocation("output_type");
        glUniform1i(outputTypeLoc, outputType);

        {
            int texturePositionsLoc = shader->GetUniformLocation("texture_position");
            glUniform1i(texturePositionsLoc, 1);
            frameBuffer->BindTexture(0, GL_TEXTURE0 + 1);
        }

        {
            int textureNormalsLoc = shader->GetUniformLocation("texture_normal");
            glUniform1i(textureNormalsLoc, 2);
            frameBuffer->BindTexture(1, GL_TEXTURE0 + 2);
        }

        {
            int textureColorLoc = shader->GetUniformLocation("texture_color");
            glUniform1i(textureColorLoc, 3);
            frameBuffer->BindTexture(2, GL_TEXTURE0 + 3);
        }

        {
            int textureDepthLoc = shader->GetUniformLocation("texture_depth");
            glUniform1i(textureDepthLoc, 4);
            frameBuffer->BindDepthTexture(GL_TEXTURE0 + 4);
        }

        {
            int textureLightLoc = shader->GetUniformLocation("texture_light");
            glUniform1i(textureLightLoc, 5);
            lightBuffer->BindTexture(0, GL_TEXTURE0 + 5);
        }

        // Render the object again but with different properties
        RenderMesh(meshes["quad"], shader, glm::vec3(0, 0, 0));
    }
}


void Lab6::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab6::LoadShader(const std::string &name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab6", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab6::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Lab6::OnKeyPress(int key, int mods)
{
    // Add key press event

    // These are the key mappings for compositing different passes.
    // What does each key seem to activate? Where can you find the
    // answer? Examine the source code to find out!
    int index = key - GLFW_KEY_0;
    if (index >= 0 && index <= 9) {
        outputType = index;
    }

    // TODO(student): Add key mappings for face culling. For example:
    // Z: off, X: front, C: back, V: both

}


void Lab6::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab6::OnWindowResize(int width, int height)
{
    // Treat window resize event
    frameBuffer->Resize(width, height, 32);
    lightBuffer->Resize(width, height, 32);
}
