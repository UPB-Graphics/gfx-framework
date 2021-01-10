#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#include <string>


namespace spg
{
    class Laborator4 : public egx::comp::SimpleScene
    {
     public:
        Laborator4();
        ~Laborator4();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        unsigned int UploadCubeMapTexture(const std::string &posx, const std::string &posy, const std::string &posz, const std::string &negx, const std::string &negy, const std::string &negz);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     private:
        int cubeMapTextureID;
    };
}   // namespace spg
