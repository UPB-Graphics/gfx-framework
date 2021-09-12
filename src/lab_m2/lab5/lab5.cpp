#include "lab_m2/lab5/lab5.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;


struct Particle
{
    glm::vec4 position;
    glm::vec4 speed;
    glm::vec4 initialPos;
    glm::vec4 initialSpeed;

    Particle() {}

    Particle(const glm::vec4 &pos, const glm::vec4 &speed)
    {
        SetInitial(pos, speed);
    }

    void SetInitial(const glm::vec4 &pos, const glm::vec4 &speed)
    {
        position = pos;
        initialPos = pos;

        this->speed = speed;
        initialSpeed = speed;
    }
};


ParticleEffect<Particle> *particleEffect;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}


void Lab5::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::quat(glm::vec3(-40 * TO_RADIANS, 0, 0)));
    camera->Update();

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load textures
    {
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "particle2.png");
    }

    LoadShader("Simple", false);
    LoadShader("Particle");

    unsigned int nrParticles = 50000;

    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());

    int cubeSize = 20;
    int hSize = cubeSize / 2;

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec4 pos(1);
        pos.x = (rand() % cubeSize - hSize) / 10.0f;
        pos.y = (rand() % cubeSize - hSize) / 10.0f;
        pos.z = (rand() % cubeSize - hSize) / 10.0f;

        glm::vec4 speed(0);
        speed.x = (rand() % 20 - 10) / 10.0f;
        speed.z = (rand() % 20 - 10) / 10.0f;
        speed.y = rand() % 2 + 2.0f;

        data[i].SetInitial(pos, speed);
    }

    particleSSBO->SetBufferData(data);
}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{
    glLineWidth(3);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    {
        auto shader = shaders["Particle"];
        if (shader->GetProgramID())
        {
            shader->Use();
            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);
        }
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    {
        glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(3, 0, 0));
        RenderMesh(meshes["box"], shaders["Simple"], model);
    }
}


void Lab5::FrameEnd()
{
#if 0
    DrawCoordinateSystem();
#endif
}


void Lab5::LoadShader(const std::string &name, bool hasGeomtery)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab5", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);
        if (hasGeomtery)
        {
            shader->AddShader(PATH_JOIN(shaderPath, name + ".GS.glsl"), GL_GEOMETRY_SHADER);
        }

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Lab5::OnKeyPress(int key, int mods)
{
}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab5::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
