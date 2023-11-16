#pragma once

#include "components/simple_scene.h"


namespace m2
{
    class Lab7 : public gfxc::SimpleScene
    {
    public:
        Lab7();
        ~Lab7();

        void Init() override;

    private:
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

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

        void BoneTransform(Mesh* mesh, float timeInSeconds);
        void ReadNodeHierarchy(Mesh* mesh, float animationTime, const aiNode* pNode, const glm::mat4& parentTransform, aiAnimation** anim);
        const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string nodeName);
        unsigned int FindRotation(float animationTime, const aiNodeAnim* pNodeAnim);
        void ComputeInterpolatedRotation(aiQuaternion& Out, float animationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindScaling(float animationTime, const aiNodeAnim* pNodeAnim);
        void ComputeInterpolatedScaling(aiVector3D& Out, float animationTime, const aiNodeAnim* pNodeAnim);
        unsigned int FindPosition(float animationTime, const aiNodeAnim* pNodeAnim);
        void ComputeInterpolatedPosition(aiVector3D& Out, float animationTime, const aiNodeAnim* pNodeAnim);

    private:

    };
}   // namespace m2
