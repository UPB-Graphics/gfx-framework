#pragma once

#include <vector>

#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"
#include "utils/glm_utils.h"


class FrameBuffer
{
 public:
    FrameBuffer();
    ~FrameBuffer();
    void Clean();
    void Generate(int width, int height, int nrTextures, bool hasDepthTexture = true, int precision = 32);
    void Resize(int width, int height, int precision = 32);

    void Bind(bool clearBuffer = true) const;
    void BindTexture(int textureID, unsigned int TextureUnit) const;
    void BindAllTextures() const;
    void BindDepthTexture(unsigned int TextureUnit) const;

    Texture2D* GetTexture(unsigned int index) const;
    Texture2D* GetDepthTexture() const;
    unsigned int GetTextureID(unsigned int index) const;
    unsigned int GetNumberOfRenderTargets() const;

    glm::ivec2 GetResolution() const;

    void SendResolution(Shader *shader) const;
    void SetClearColor(glm::vec4 clearColor);

    static void Clear();
    static void BindDefault();
    static void BindDefault(const glm::ivec2 &viewportSize, bool clearBuffer = false);
    static void SetViewport(const glm::ivec2 &viewportSize, const glm::ivec2 offset = glm::ivec2(0, 0));
    static void SetDefaultClearColor(glm::vec4 clearColor);

 private:
    Texture2D *textures;
    Texture2D *depthTexture;

    unsigned int FBO;
    unsigned int *DrawBuffers;          // TODO(developer): test if is necessary to declare

    int width;
    int height;
    unsigned int nrTextures;
    glm::vec4 clearColor;
    static glm::vec4 defaultClearColor;
};
