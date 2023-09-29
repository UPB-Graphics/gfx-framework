#include "lab_m2/lab3/lab3.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab3::Lab3()
{
    angle = 0;

    light_position = glm::vec3(7, 7, 7);
    light_direction = glm::normalize(glm::vec3(-1, -1, -1));

    light_space_near_plane = 0.1f;
    light_space_far_plane = 30.0f;

    draw_framebuffer_textures = true;

    framebuffer_object = 0;
    color_texture = 0;
    depth_texture = 0;
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(-1, 7, 11), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
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

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create the shaders for rendering the scene from the
    // perspective of the light source, drawing textures from
    // the framebuffer on the screen and shadows
    LoadShader("ShadowMappingPassOne");
    LoadShader("ShadowMappingPassTwo");
    LoadShader("ViewColorTexture");
    LoadShader("ViewDepthTexture");

    // Load the ground texture
    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "ground.jpg");

    // Create the framebuffer on which the scene is rendered
    // from the perspective of the spot light source
    CreateFramebuffer(1024, 1024);
}

void Lab3::CreateFramebuffer(int width, int height)
{
    // TODO(student): In this method, use the attributes
    // 'framebuffer_object', 'color_texture' and 'depth_texture'
    // declared in lab3.h

    // TODO(student): Generate and bind the framebuffer

    // TODO(student): Generate, bind and initialize the color texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // TODO(student): Bind the color texture to the
    // framebuffer as a color attachment at position 0

    // TODO(student): Generate, bind and initialize the depth texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // TODO(student): Bind the depth texture to the framebuffer as a depth attachment

    // TODO(student): Set the color texture as the draw texture

    // TODO(student): Check the status of the framebuffer

    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Lab3::FrameStart()
{
    ClearScreen();
}

void Lab3::Update(float deltaTimeSeconds)
{
    angle += 0.5f * deltaTimeSeconds;

    // Save camera position and rotation
    auto camera = GetSceneCamera();

    glm::vec3 camPosition = camera->m_transform->GetWorldPosition();
    glm::quat camRotation = camera->m_transform->GetWorldRotation();
    auto projectionInfo = camera->GetProjectionInfo();

    // Render the scene from the perspective of the spot light
    // source. It uses a perspective projection centered in the
    // position of the light source with the view direction in
    // the direction of illumination of the source.
    {
        camera->SetPosition(light_position);
        camera->SetRotation(glm::quatLookAt(light_direction, glm::vec3(0, 1, 0)));

        // The spot light source has an angle opening of 90
        // degrees, so a perspective projection is used with
        // a viewing angle of 90 degrees both vertically and
        // horizontally. For this reason, an aspect ratio of
        // 1 is used.
        camera->SetPerspective(90 /* vert fov */, 1 /* aspect ratio */,
            light_space_near_plane, light_space_far_plane);

        // Save the view and projection matrix
        light_space_view = camera->GetViewMatrix();
        light_space_projection = camera->GetProjectionMatrix();

        // TODO(student): Bind the framebuffer created before
        // and clear the color and depth textures

        // TODO(student): Use glViewport to specify the render
        // area whose size (width, height) is the resolution of
        // the textures in the framebuffer
        //
        // glViewport(start_X, start_Y, width, height);

        // TODO(student): Use DrawScene to render the objects
        // with "ShadowMappingPassOne" shader
    }

    // Render the scene with shadows
    {
        camera->SetPosition(camPosition);
        camera->SetRotation(camRotation);
        camera->SetProjection(projectionInfo);

        // TODO(student): Bind the default framebuffer
        // and clear the color and depth textures

        // TODO(student): Use glViewport to specify the render
        // area whose size is the resolution of the window

        DrawScene(shaders["ShadowMappingPassTwo"]);
    }

    // Render the point light in the scene
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, light_position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    }

    if (draw_framebuffer_textures) {
        // Render on the screen the color and depth textures
        // of the previously created framebuffer
        DrawFramebufferTextures();
    }
}

void Lab3::DrawScene(Shader *shader)
{
    for (int i = 0; i < 16; i++)
    {
        float rotateAngle = (angle + i) * ((i % 2) * 2 - 1);
        glm::vec3 position = glm::vec3(-4 + (i % 4) * 2.5, 0, -2 + (i / 4) * 2.5);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
        modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));

        RenderSimpleMesh(meshes["bamboo"], shader, modelMatrix, TextureManager::GetTexture("bamboo.png"));
    }

    glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3 (0.5f));

    RenderSimpleMesh(meshes["plane"], shader, modelMatrix, TextureManager::GetTexture("ground.jpg"));
}

void Lab3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(loc_light_position, light_position.x, light_position.y, light_position.z);

    GLint loc_light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(loc_light_direction, light_direction.x, light_direction.y, light_direction.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    GLint eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set model matrix uniform
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Set view matrix uniform
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Set projection matrix uniform
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Set light space view matrix uniform
    GLint loc_light_space_view = glGetUniformLocation(shader->program, "light_space_view");
    glUniformMatrix4fv(loc_light_space_view, 1, GL_FALSE, glm::value_ptr(light_space_view));

    // Set light space projection matrix uniform
    GLint loc_light_space_projection = glGetUniformLocation(shader->program, "light_space_projection");
    glUniformMatrix4fv(loc_light_space_projection, 1, GL_FALSE, glm::value_ptr(light_space_projection));

    // Set uniform for the far plane of the
    // projection transformation in the light space
    GLint loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, light_space_far_plane);

    // Set texture uniform
    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // TODO(student): Activate texture location 1, bind
    // the depth texture and send the uniform value

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Lab3::DrawFramebufferTextures()
{
    int screenPosX = window->GetResolution().x - 950;
    int screenPosY = 100;

    int width = 400;
    int height = 400;

    if (color_texture)
    {
        // Render the color texture on the screen
        glViewport(screenPosX, screenPosY, width, height);

        RenderTextureScreen(shaders["ViewColorTexture"], color_texture);
    }

    if (depth_texture)
    {
        // Render the depth texture on the screen
        glViewport(screenPosX + 450, screenPosY, width, height);

        RenderTextureScreen(shaders["ViewDepthTexture"], depth_texture);
    }
}

void Lab3::RenderTextureScreen(Shader *shader, unsigned int textureID)
{
    if (!shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set uniforms for near and far plane of the
    // projection transformation in the light space
    GLint loc_light_space_near_plane = glGetUniformLocation(shader->program, "light_space_near_plane");
    glUniform1f(loc_light_space_near_plane, light_space_near_plane);

    GLint loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, light_space_far_plane);

    // Set texture uniform
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    // Draw the object
    glBindVertexArray(meshes["quad"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["quad"]->GetDrawMode(), static_cast<int>(meshes["quad"]->indices.size()), GL_UNSIGNED_INT, 0);
}

void Lab3::FrameEnd()
{

}

void Lab3::LoadShader(const std::string& name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab3", "shaders");

    // Create a shader program
    {
        Shader* shader = new Shader(name);
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

void Lab3::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) light_position -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) light_position -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) light_position += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) light_position += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) light_position += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) light_position -= up * deltaTime * speed;
    }
}

void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event

    // Switch between viewing and hiding the color
    // and depth textures in the framebuffer
    if (key == GLFW_KEY_F1) {
        draw_framebuffer_textures = !draw_framebuffer_textures;
    }
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab3::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
