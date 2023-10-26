#include "lab_m2/lab4/lab4.h"

#include <vector>
#include <iostream>
#include <limits>

using namespace std;
using namespace m2;


struct Particle
{
    glm::vec4 position;
    glm::vec4 speed;
    glm::vec4 initialPos;
    glm::vec4 initialSpeed;
    float delay;
    float initialDelay;
    float lifetime;
    float initialLifetime;

    Particle() {}

    Particle(const glm::vec4 &pos, const glm::vec4 &speed)
    {
        SetInitial(pos, speed);
    }

    void SetInitial(const glm::vec4 &pos, const glm::vec4 &speed,
        float delay = 0, float lifetime = 0)
    {
        position = pos;
        initialPos = pos;

        this->speed = speed;
        initialSpeed = speed;

        this->delay = delay;
        initialDelay = delay;

        this->lifetime = lifetime;
        initialLifetime = lifetime;
    }
};


ParticleEffect<Particle> *particleEffect;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
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

        // TODO(student): Load images "rain.png", "snowflake.png" and "fire.png" as
        // textures, similar to "particle2.png", loaded above. The images can be
        // found in the same directory as "particle2.png"

    }

    LoadShader("Fireworks", "Particle_fireworks", "Particle_simple", "Particle", true);
    LoadShader("RainSnow", "Particle_rain_snow", "Particle_simple", "Particle", true);
    LoadShader("Fire", "Particle_fire", "Particle_multiple_textures", "Particle", true);

    ResetParticlesFireworks(20,20,20);

    generator_position = glm::vec3(0, 0, 0);
    scene = 0;
    offset = 0.05;
}

void Lab4::ResetParticlesFireworks(int xSize, int ySize, int zSize)
{
    unsigned int nrParticles = 5000;

    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());

    int xhSize = xSize / 2;
    int yhSize = ySize / 2;
    int zhSize = zSize / 2;

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec4 pos(1);
        pos.x = (rand() % xSize - xhSize) / 10.0f;
        pos.y = (rand() % ySize - yhSize) / 10.0f;
        pos.z = (rand() % zSize - zhSize) / 10.0f;

        glm::vec4 speed(0);
        speed.x = (rand() % 20 - 10) / 10.0f;
        speed.z = (rand() % 20 - 10) / 10.0f;
        speed.y = rand() % 2 + 2.0f;

        data[i].SetInitial(pos, speed);
    }

    particleSSBO->SetBufferData(data);
}

void Lab4::ResetParticlesRainSnow(int xSize, int ySize, int zSize)
{
    unsigned int nrParticles = 5000;

    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());


    int xhSize = xSize / 2;
    int yhSize = ySize / 2;
    int zhSize = zSize / 2;

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec4 pos(1);
        pos.x = (rand() % xSize - xhSize) / 10.0f;
        pos.y = (rand() % ySize - yhSize) / 10.0f;
        pos.z = (rand() % zSize - zhSize) / 10.0f;

        glm::vec4 speed(0);
        speed.x = - (rand() % 20 - 10) / 10.0f;
        speed.z = - (rand() % 20 - 10) / 10.0f;
        speed.y = - (rand() % 2 + 2.0f);

        float delay = (rand() % 100 / 100.0f) * 3.0f;

        data[i].SetInitial(pos, speed, delay);
    }

    particleSSBO->SetBufferData(data);
}


void Lab4::ResetParticlesFire(float radius)
{
    unsigned int nrParticles = 5000;

    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec3 pos(1);
        pos.x = (rand() % 100 - 50)/ 100.0f ;
        pos.y = (rand() % 100 - 50)/ 100.0f;
        pos.z = (rand() % 100 - 50)/ 100.0f;
        pos = glm::normalize(pos) * radius ;

        glm::vec3 speed(0);
        speed = glm::normalize(glm::vec3(0, 5, 0) - glm::vec3(pos));
        speed *= (rand() % 100 / 100.0f);
        speed += glm::vec3(rand() % 5 / 5.0f, rand() % 5 / 5.0f, rand() % 5 / 5.0f) * 0.2f;

        float lifetime = 1 + (rand() % 100 / 100.0f);

        data[i].SetInitial(glm::vec4 (pos, 1), glm::vec4 (speed, 0), 0, lifetime);
    }

    particleSSBO->SetBufferData(data);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    if (scene == 0)
    {
        auto shader = shaders["Fireworks"];
        if (shader->GetProgramID())
        {
            shader->Use();

            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);

            // TODO(student): Send uniforms generator_position,
            // deltaTime and offset to the shader

        }
    }

    if (scene == 1)
    {
        auto shader = shaders["RainSnow"];
        if (shader->GetProgramID())
        {
            shader->Use();

            // TODO(student): Send correct texture for snow
            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);

            // TODO(student): Send uniforms generator_position,
            // deltaTime and offset to the shader

        }
    }

    if (scene == 2)
    {
        auto shader = shaders["RainSnow"];
        if (shader->GetProgramID())
        {
            shader->Use();

            // TODO(student): Send correct texture for rain
            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);

            // TODO(student): Send uniforms generator_position,
            // deltaTime and offset to the shader

        }
    }

    if (scene == 3)
    {
        auto shader = shaders["Fire"];
        if (shader->GetProgramID())
        {
            shader->Use();
            // TODO(student): Send correct texture for fire
            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);

            // TODO(student): Send uniforms generator_position,
            // deltaTime and offset to the shader

        }
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    {
        glm::mat4 model = glm::translate(glm::mat4(1), generator_position);
        if (scene == 1 || scene == 2)
            model = glm::scale(model, glm::vec3(10,0.5,0.5));
        else
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        RenderMesh(meshes["box"], shaders["Simple"], model);
    }
}


void Lab4::FrameEnd()
{
#if 0
    DrawCoordinateSystem();
#endif
}


void Lab4::LoadShader(const std::string& name, const std::string &VS, const std::string& FS, const std::string& GS,  bool hasGeomtery)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab4", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, VS + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, FS + ".FS.glsl"), GL_FRAGMENT_SHADER);
        if (hasGeomtery)
        {
            shader->AddShader(PATH_JOIN(shaderPath, GS + ".GS.glsl"), GL_GEOMETRY_SHADER);
        }

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
        const float speed = 2;

        if (window->KeyHold(GLFW_KEY_A))
            generator_position.x -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D))
            generator_position.x += deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_W))
            generator_position.y += deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S))
            generator_position.y -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_R))
            generator_position.z -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_F))
            generator_position.z += deltaTime * speed;
    }

    if (window->KeyHold(GLFW_KEY_Z))
        offset += deltaTime * 0.1;
    if (window->KeyHold(GLFW_KEY_X))
        offset -= deltaTime * 0.1;


}


void Lab4::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_1)
    {
        scene = 0;
        ResetParticlesFireworks(20,20,20);
        generator_position = glm::vec3(0, 0, 0);
    }
    if (key == GLFW_KEY_2)
    {
        scene = 1;
        ResetParticlesRainSnow(100,10,10);
        generator_position = glm::vec3(0, 0, 0);
    }
    if (key == GLFW_KEY_3)
    {
        scene = 2;
        ResetParticlesRainSnow(100, 10, 10);
        generator_position = glm::vec3(0, 0, 0);
    }
    if (key == GLFW_KEY_4)
    {
        scene = 3;
        ResetParticlesFire(0.25);
        generator_position = glm::vec3(0, 0, 0);
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab4::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
