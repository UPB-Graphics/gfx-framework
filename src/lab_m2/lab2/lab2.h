#pragma once

#include "components/simple_scene.h"


namespace m2
{
    class Lab2 : public gfxc::SimpleScene
    {
     public:
        Lab2();
        ~Lab2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        // Info about the generated surfaces
        glm::vec3 control_p0, control_p1, control_p2, control_p3;
        unsigned int no_of_generated_points, no_of_instances;
        float max_translate, max_rotate;
    };
}   // namespace m2
