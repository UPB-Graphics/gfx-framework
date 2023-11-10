#include "lab_m2/lab6/lab6.h"

#include <vector>
#include <iostream>

#include "stb/stb_image.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab6::Lab6()
{
    framebuffer_object = 0;
    color_texture = 0;
    depth_texture = 0;

    angle = 0;

    type = 0;
}


Lab6::~Lab6()
{
}


void Lab6::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, -1, 4), glm::quat(glm::vec3(RADIANS(10), 0, 0)));
    camera->Update();

    std::string texturePath = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube");
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab6", "shaders");

    {
        Mesh* mesh = new Mesh("bunny");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("archer");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters", "archer"), "Archer.fbx");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for rendering cubemap texture
    {
        Shader *shader = new Shader("CubeMap");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for standard rendering
    {
        Shader *shader = new Shader("ShaderNormal");
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for creating a CUBEMAP
    {
        Shader *shader = new Shader("Framebuffer");
        shader->AddShader(PATH_JOIN(shaderPath, "Framebuffer.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Framebuffer.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Framebuffer.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    cubeMapTextureID = UploadCubeMapTexture(
        PATH_JOIN(texturePath, "pos_x.png"),
        PATH_JOIN(texturePath, "pos_y.png"),
        PATH_JOIN(texturePath, "pos_z.png"),
        PATH_JOIN(texturePath, "neg_x.png"),
        PATH_JOIN(texturePath, "neg_y.png"),
        PATH_JOIN(texturePath, "neg_z.png"));

    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS), "characters", "archer", "Akai_E_Espiritu.fbm", "akai_diffuse.png");

    // Create the framebuffer on which the scene is rendered from the perspective of the mesh
    // Texture size must be cubic
     CreateFramebuffer(1024, 1024);
}


void Lab6::FrameStart()
{
}


void Lab6::Update(float deltaTimeSeconds)
{
    angle += 0.5f * deltaTimeSeconds;

    auto camera = GetSceneCamera();

    // Draw the scene in Framebuffer
    if (framebuffer_object)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
        // Set the clear color for the color buffer
        glClearColor(0,0,0, 1);
        // Clears the color buffer (using the previously set color) and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 1024, 1024);

        Shader *shader = shaders["Framebuffer"];
        shader->Use();

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

        {
            glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

            glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
            glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
            glUniform1i(glGetUniformLocation(shader->program, "texture_cubemap"), 1);

            glUniform1i(glGetUniformLocation(shader->program, "cube_draw"), 1);

            meshes["cube"]->Render();
        }

        for (int i = 0; i < 5; i++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= glm::rotate(glm::mat4(1), angle + i * glm::radians(360.0f) / 5, glm::vec3(0, 1, 0));
            modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(3, -1, 0));
            modelMatrix *= glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
            modelMatrix *= glm::scale(glm::mat4(1), glm::vec3(0.01f));

            glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

            glm::mat4 cubeView[6] =
            {
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +X
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // -X
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)), // -Y
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +Z
                glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // -Z
            };

            glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));
            glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TextureManager::GetTexture("Akai_E_Espiritu.fbm\\akai_diffuse.png")->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

            glUniform1i(glGetUniformLocation(shader->program, "cube_draw"), 0);

            meshes["archer"]->Render();
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, color_texture);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        //reset drawing to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);

    // Draw the cubemap
    {
        Shader* shader = shaders["ShaderNormal"];
        shader->Use();

        glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        int loc_texture = shader->GetUniformLocation("texture_cubemap");
        glUniform1i(loc_texture, 0);

        meshes["cube"]->Render();
    }

    // Draw five archers around the mesh
    for (int i = 0; i < 5; i++)
    {
        Shader* shader = shaders["Simple"];
        shader->Use();

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= glm::rotate(glm::mat4(1), angle + i * glm::radians(360.0f) / 5, glm::vec3(0, 1, 0));
        modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(3, -1, 0));
        modelMatrix *= glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
        modelMatrix *= glm::scale(glm::mat4(1), glm::vec3(0.01f));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureManager::GetTexture("Akai_E_Espiritu.fbm\\akai_diffuse.png")->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

        meshes["archer"]->Render();
    }

    // Draw the reflection on the mesh
    {
        Shader *shader = shaders["CubeMap"];
        shader->Use();

        glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.1f));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        auto cameraPosition = camera->m_transform->GetWorldPosition();

        if (!color_texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
            int loc_texture = shader->GetUniformLocation("texture_cubemap");
            glUniform1i(loc_texture, 0);
        }

        if (color_texture) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, color_texture);
            int loc_texture2 = shader->GetUniformLocation("texture_cubemap");
            glUniform1i(loc_texture2, 1);
        }
        

        int loc_camera = shader->GetUniformLocation("camera_position");
        glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);
        
        glUniform1i(shader->GetUniformLocation("type"), type);

        meshes["bunny"]->Render();
    }
}


void Lab6::FrameEnd()
{
    // DrawCoordinateSystem();
}


unsigned int Lab6::UploadCubeMapTexture(const std::string &pos_x, const std::string &pos_y, const std::string &pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z)
{
    int width, height, chn;

    unsigned char* data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    unsigned int textureID = 0;
    // TODO(student): Create the texture

    // TODO(student): Bind the texture

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (GLEW_EXT_texture_filter_anisotropic) {
        float maxAnisotropy;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // TODO(student): Load texture information for each face

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
    }

    // Free memory
    SAFE_FREE(data_pos_x);
    SAFE_FREE(data_pos_y);
    SAFE_FREE(data_pos_z);
    SAFE_FREE(data_neg_x);
    SAFE_FREE(data_neg_y);
    SAFE_FREE(data_neg_z);

    return textureID;
}

void Lab6::CreateFramebuffer(int width, int height)
{
    // TODO(student): In this method, use the attributes
    // 'framebuffer_object', 'color_texture'
    // declared in lab6.h

    // TODO(student): Generate and bind the framebuffer


    // TODO(student): Generate and bind the color texture


    // TODO(student): Initialize the color textures



    if (color_texture) {
        //cubemap params
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (GLEW_EXT_texture_filter_anisotropic) {
            float maxAnisotropy;

            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Bind the color textures to the framebuffer as a color attachments
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        std::vector<GLenum> draw_textures;
        draw_textures.push_back(GL_COLOR_ATTACHMENT0);
        glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    }

    // TODO(student): Generate and bind the depth texture


    // TODO(student): Initialize the depth textures


    if (depth_texture) {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);
    }

    glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    if (key == GLFW_KEY_1)
    {
        type=1;
    }

    if (key == GLFW_KEY_2)
    {
        type=0;
    }
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
}
