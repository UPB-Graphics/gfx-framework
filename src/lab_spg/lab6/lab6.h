#pragma once

#include <string>
#include <vector>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "core/gpu/frame_buffer.h"


namespace spg
{
    struct LightInfo
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::mat4 lightArea;
        float radius;
    };

    class Laborator6 : public egx::comp::SimpleScene
    {
     public:
        Laborator6();
        ~Laborator6();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void LoadShader(std::string fileName);

     private:
        FrameBuffer *frameBuffer;
        FrameBuffer *lightBuffer;
        std::vector<LightInfo> lights;
        int outputType;

        // TODO(student): Declare any class members you might need.

    };
}   // namespace spg
