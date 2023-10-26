#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/particle_effect.h"


namespace m2
{
    class Lab4 : public gfxc::SimpleScene
    {
     public:
        Lab4();
        ~Lab4();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void LoadShader(const std::string& name, 
            const std::string& VS, const std::string& FS, const std::string& GS="",
            bool hasGeomtery = false);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void ResetParticlesFireworks(int xSize, int ySize, int zSize);
        void ResetParticlesRainSnow(int xSize, int ySize, int zSize);
        void ResetParticlesFire(float radius);

     protected:
        glm::mat4 modelMatrix;
        glm::vec3 generator_position;
        GLenum polygonMode;
        int scene;
        float offset;

    };
}   // namespace m2
