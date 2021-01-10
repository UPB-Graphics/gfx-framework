#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"


#if defined(_WIN32)
#   define WIN_API_FILE_BROWSING
#endif


namespace spg
{
    class Laborator7 : public SimpleScene
    {
     public:
        Laborator7();
        ~Laborator7();

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

        void OpenDialogue();
        void OnFileSelected(std::string fileName);

        // Processing Effects
        void GrayScale();
        void SaveImage(std::string fileName);

     private:
        Texture2D *originalImage;
        Texture2D *processedImage;

        bool flip;
        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;
    };
}   // namespace spg
