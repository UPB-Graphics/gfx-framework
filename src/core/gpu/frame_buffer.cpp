#include "core/gpu/frame_buffer.h"

#include <iostream>
#include <utility>

#include "core/window/window_callbacks.h"
#include "utils/gl_utils.h"
#include "utils/memory_utils.h"


glm::vec4 FrameBuffer::defaultClearColor = glm::vec4(0);


FrameBuffer::FrameBuffer()
{
    FBO = 0;
    depthTexture = nullptr;
    textures = nullptr;
    DrawBuffers = nullptr;
    clearColor = glm::vec4(0, 0, 0, 1);
}


FrameBuffer::~FrameBuffer()
{
    SAFE_FREE(depthTexture);
}


void FrameBuffer::Clean()
{
    if (FBO)
        glDeleteFramebuffers(1, &FBO);
    SAFE_FREE_ARRAY(textures);
    SAFE_FREE_ARRAY(DrawBuffers)
}


void FrameBuffer::Generate(int width, int height, int nrTextures, bool hasDepthTexture, int precision)
{
    Clean();

    precision = (precision / 8) * 8;

    #ifdef DEBUG_INFO
        cout << "FBO: " << width << " * " << height << " textures attached: " << nrTextures << endl;
    #endif

    this->width = width;
    this->height = height;
    this->nrTextures = nrTextures;

    // Create FrameBufferObject
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    if (nrTextures > 0) {
        DrawBuffers = new GLenum[nrTextures];

        // Add attachments to drawing buffer
        for (int i = 0; i < nrTextures; i++)
            DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

        // Create attached textures
        textures = new Texture2D[nrTextures];
        for (int i = 0; i < nrTextures; i++)
        {
            textures[i].CreateFrameBufferTexture(width, height, i, precision);
        }

        glDrawBuffers(nrTextures, DrawBuffers);
    }

    // Create depth texture
    if (hasDepthTexture) {
        depthTexture = new Texture2D();
        depthTexture->CreateDepthBufferTexture(width, height);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "FRAMEBUFFER NOT COMPLETE" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CheckOpenGLError();
}


void FrameBuffer::Resize(int width, int height, int precision)
{
    this->width = width;
    this->height = height;
    precision = (precision / 8) * 8;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    for (unsigned int i = 0; i < nrTextures; i++)
    {
        textures[i].CreateFrameBufferTexture(width, height, i, precision);
    }

    if (depthTexture) {
        depthTexture->CreateDepthBufferTexture(width, height);
    }
}


void FrameBuffer::Bind(bool clearBuffer) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);
    if (clearBuffer) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}


void FrameBuffer::SendResolution(Shader *shader) const
{
    glUniform2i(shader->loc_resolution, width, height);
}


void FrameBuffer::SetClearColor(glm::vec4 clearColor)
{
    this->clearColor = std::move(clearColor);
}


glm::ivec2 FrameBuffer::GetResolution() const {
    return glm::ivec2(width, height);
}


unsigned int FrameBuffer::GetNumberOfRenderTargets() const
{
    return nrTextures;
}


void FrameBuffer::BindTexture(int textureID, unsigned int TextureUnit) const
{
    textures[textureID].BindToTextureUnit(TextureUnit);
}


void FrameBuffer::BindDepthTexture(unsigned int TextureUnit) const
{
    depthTexture->BindToTextureUnit(TextureUnit);
}


Texture2D* FrameBuffer::GetTexture(unsigned int index) const
{
    return &textures[index];
}


Texture2D* FrameBuffer::GetDepthTexture() const
{
    return depthTexture;
}


unsigned int FrameBuffer::GetTextureID(unsigned int index) const
{
    return textures[index].GetTextureID();
}


void FrameBuffer::BindAllTextures() const
{
    for (unsigned int i = 0; i < nrTextures; i++) {
        textures[i].BindToTextureUnit(GL_TEXTURE0 + i);
    }
}


void FrameBuffer::BindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::BindDefault(const glm::ivec2 &viewportSize, bool clearBuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, viewportSize.x, viewportSize.y);
    if (clearBuffer) {
        glClearColor(defaultClearColor.r, defaultClearColor.g, defaultClearColor.b, defaultClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    CheckOpenGLError();
}


void FrameBuffer::SetViewport(const glm::ivec2 & viewportSize, const glm::ivec2 offset)
{
    glViewport(offset.x, offset.y, viewportSize.x, viewportSize.y);
}


void FrameBuffer::SetDefaultClearColor(glm::vec4 clearColor)
{
    defaultClearColor = clearColor;
}


void FrameBuffer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
