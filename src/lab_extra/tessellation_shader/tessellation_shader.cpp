#include "lab_extra/tessellation_shader/tessellation_shader.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace extra;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


TessellationShader::TessellationShader()
{
}


TessellationShader::~TessellationShader()
{
}


void TessellationShader::Init()
{
    ToggleGroundPlane();

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        const string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::EXTRA, "tessellation_shader", "shaders");
        Shader *shader = new Shader("TessShader");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "TessellationControl.glsl"), GL_TESS_CONTROL_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "TessellationEval.glsl"), GL_TESS_EVALUATION_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    tessFactorInner = 1;
    tessFactorOuter = 1;

    polygonMode = GL_LINE;
}


void TessellationShader::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TessellationShader::Update(float deltaTimeSeconds)
{
    ClearScreen();

    {
        auto camera = GetSceneCamera();
        gfxc::Transform T;
        T.SetWorldRotation(glm::vec3(90, 0, 0));

        auto shader = shaders["TessShader"];
        shader->Use();

        glLineWidth(3);
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

        // Render an object using the specified shader and the specified position
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(T.GetModel()));

        int loc_tess_factor_inner = shader->GetUniformLocation("tess_factor_inner");
        int loc_tess_factor_outer = shader->GetUniformLocation("tess_factor_outer");

        glUniform1i(loc_tess_factor_inner, tessFactorInner);
        glUniform1i(loc_tess_factor_outer, tessFactorOuter);

        int loc_time = shader->GetUniformLocation("time");
        glUniform1f(loc_time, (float)Engine::GetElapsedTime());

        auto mesh = meshes["quad"];
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
#if 0
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#else
        glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
#endif
    }
}


void TessellationShader::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TessellationShader::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void TessellationShader::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_0)
    {
        tessFactorInner++;
        cout << "Tess factor inner: " << tessFactorInner << endl;
    }

    if (key == GLFW_KEY_9)
    {
        if (tessFactorInner > 1) tessFactorInner--;
        cout << "Tess factor inner: " << tessFactorInner << endl;
    }

    if (key == GLFW_KEY_EQUAL)
    {
        tessFactorOuter++;
        cout << "Tess factor outer: " << tessFactorOuter << endl;
    }

    if (key == GLFW_KEY_MINUS)
    {
        if (tessFactorOuter > 1) tessFactorOuter--;
        cout << "Tess factor outer: " << tessFactorOuter << endl;
    }

    if (key == GLFW_KEY_F)
    {
        switch (polygonMode)
        {
            case GL_FILL:
                polygonMode = GL_LINE;
                break;

            case GL_LINE:
                polygonMode = GL_FILL;
                break;

            default:
                break;
        }
    }
}


void TessellationShader::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TessellationShader::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TessellationShader::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TessellationShader::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void TessellationShader::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void TessellationShader::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
