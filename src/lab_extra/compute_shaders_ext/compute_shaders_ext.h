#pragma once

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"


namespace extra
{
    class ComputeShadersExt : public gfxc::SimpleScene
    {
     public:
        ComputeShadersExt();
        ~ComputeShadersExt();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        FrameBuffer *frameBuffer;
        FrameBuffer *frameBufferBlur;
        Texture2D *textureBlur;
        float angle = 0;
        int textureID = 0;
        bool fullScreenPass = true;
    };
}   // namespace extra
