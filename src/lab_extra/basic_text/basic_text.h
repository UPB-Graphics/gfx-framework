#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace extra
{
    class BasicText : public gfxc::SimpleScene
    {
     public:
        BasicText();
        ~BasicText();

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

        // Draw the heads-up display
        void DrawHUD();

     private:
        // The actual renderer object
        gfxc::TextRenderer *textRenderer;

        // Draw primitives mode
        GLenum polygonMode;

        // Colors
        const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);
        const glm::vec3 kBackgroundColor = NormalizedRGB(41, 45, 62);
    };
}   // namespace extra
