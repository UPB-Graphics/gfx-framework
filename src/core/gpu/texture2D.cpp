#include "core/gpu/texture2D.h"

#include <thread>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "utils/memory_utils.h"


void write_image_thread(const char* fileName, unsigned int width, unsigned int height, unsigned int channels, const unsigned char *data)
{
    stbi_write_png(fileName, width, height, channels, data, width * channels);
    delete data;
}


const GLint pixelFormat[5] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
const GLint internalFormat[][5] = {
    { 0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 },
    { 0, GL_R16, GL_RG16, GL_RGB16, GL_RGBA16 },
    { 0, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F },
    { 0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F }
};


Texture2D::Texture2D()
{
    width = 0;
    height = 0;
    channels = 0;
    textureID = 0;
    bitsPerPixel = 8;
    cacheInMemory = false;
    targetType = GL_TEXTURE_2D;
    wrappingMode = GL_REPEAT;
    textureMinFilter = GL_LINEAR;
    textureMagFilter = GL_LINEAR;
}


Texture2D::~Texture2D() {
}


GLuint Texture2D::GetTextureID() const
{
    return textureID;
}


void Texture2D::Init(GLuint gpuTextureID, unsigned int width, unsigned int height, unsigned int channels)
{
    this->textureID = gpuTextureID;
    this->width = width;
    this->height = height;
    this->channels = channels;
}


bool Texture2D::Load2D(const char *fileName, GLenum wrapping_mode)
{
    int width, height, chn;
    imageData = stbi_load(fileName, &width, &height, &chn, 0);

    if (imageData == NULL) {
#ifdef DEBUG_INFO
        cout << "ERROR loading texture: " << file_name << endl << endl;
#endif
        return false;
    }

#ifdef DEBUG_INFO
    cout << "Loaded " << file_name << endl;
    cout << width << " * " << height << " channels: " << chn << endl << endl;
#endif

    textureMinFilter = GL_LINEAR_MIPMAP_LINEAR;
    wrappingMode = wrapping_mode;

    Init2DTexture(width, height, chn);
    glTexImage2D(targetType, 0, internalFormat[0][chn], width, height, 0, pixelFormat[chn], GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(targetType);
    glBindTexture(targetType, 0);
    CheckOpenGLError();

    if (cacheInMemory == false)
    {
        stbi_image_free(imageData);
    }

    return true;
}


void Texture2D::SaveToFile(const char *fileName)
{
    if (imageData == nullptr)
    {
        imageData = new unsigned char[width * height * channels];
    }
    glBindTexture(targetType, textureID);
    glGetTexImage(targetType, 0, pixelFormat[channels], GL_UNSIGNED_BYTE, (void *)imageData);

    stbi_write_png(fileName, width, height, channels, imageData, width * channels);
}


void Texture2D::CacheInMemory(bool state)
{
    cacheInMemory = state;
}


void Texture2D::UploadNewData(const unsigned char *img)
{
    Bind();
    glTexSubImage2D(targetType, 0, 0, 0, width, height, pixelFormat[channels], GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(targetType);
    UnBind();
}


void Texture2D::UploadNewData(const unsigned int *img)
{
    Bind();
    glTexSubImage2D(targetType, 0, 0, 0, width, height, pixelFormat[channels], GL_UNSIGNED_INT, img);
    glGenerateMipmap(targetType);
    UnBind();
}


void Texture2D::Create(const unsigned char *img, int width, int height, int chn)
{
    Init2DTexture(width, height, chn);
    glTexImage2D(targetType, 0, internalFormat[0][chn], width, height, 0, pixelFormat[chn], GL_UNSIGNED_BYTE, (void *)img);
    UnBind();
}


void Texture2D::CreateU16(const unsigned int *img, int width, int height, int chn)
{
    Init2DTexture(width, height, chn);
    glTexImage2D(targetType, 0, internalFormat[1][chn], width, height, 0, pixelFormat[chn], GL_UNSIGNED_INT, (void *)img);
    UnBind();
}


void Texture2D::CreateCubeTexture(const float *data, unsigned int width, unsigned int height, unsigned int chn)
{
    this->width = width;
    this->height = height;
    targetType = GL_TEXTURE_CUBE_MAP;

    glDeleteTextures(1, &textureID);
    glGenTextures(1, &textureID);

    glBindTexture(targetType, textureID);
    glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, textureMagFilter);
    glTexParameteri(targetType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(targetType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(targetType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

#if 0
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, internalFormat[3][chn], width, height);
#endif

    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat[3][chn], width, height, 0, pixelFormat[chn], GL_FLOAT, NULL);
    }

    UnBind();
}


void Texture2D::CreateFrameBufferTexture(unsigned int width, unsigned int height, unsigned int targetID, unsigned int precision)
{
    bitsPerPixel = precision;
    int prec = precision / 8 - 1;
    Init2DTexture(width, height, 4);
    glTexImage2D(targetType, 0, internalFormat[prec][4], width, height, 0, pixelFormat[4], GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targetID, GL_TEXTURE_2D, textureID, 0);
    UnBind();
}


void Texture2D::CreateDepthBufferTexture(unsigned int width, unsigned int height)
{
    Init2DTexture(width, height, 1);
    glTexImage2D(targetType, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
    UnBind();
}


void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}


void Texture2D::BindToTextureUnit(GLenum TextureUnit) const
{
    if (!textureID) return;
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}


void Texture2D::UnBind() const
{
    glBindTexture(targetType, 0);
    CheckOpenGLError();
}


void Texture2D::SetTextureParameters()
{
    glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, textureMagFilter);
    glTexParameteri(targetType, GL_TEXTURE_WRAP_S, wrappingMode);
    glTexParameteri(targetType, GL_TEXTURE_WRAP_T, wrappingMode);
    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(targetType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
}


unsigned int Texture2D::GetWidth() const
{
    return width;
}


unsigned int Texture2D::GetHeight() const
{
    return height;
}


void Texture2D::GetSize(unsigned int &width, unsigned int &height) const
{
    width = this->width;
    height = this->height;
}


unsigned char *Texture2D::GetImageData() const
{
    return imageData;
}


unsigned int Texture2D::GetNrChannels() const
{
    return channels;
}


void Texture2D::SetWrappingMode(GLenum mode)
{
    if (wrappingMode == mode)
        return;

    wrappingMode = mode;

    if (textureID)
    {
        glBindTexture(targetType, textureID);
        glTexParameteri(targetType, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(targetType, GL_TEXTURE_WRAP_T, mode);
        glTexParameteri(targetType, GL_TEXTURE_WRAP_R, mode);
    }
    CheckOpenGLError();
}


void Texture2D::SetFiltering(GLenum minFilter, GLenum magFilter)
{
    if (textureID)
    {
        glBindTexture(targetType, textureID);

        if (textureMinFilter != minFilter) {
            glTexParameteri(targetType, GL_TEXTURE_MIN_FILTER, minFilter);
            textureMinFilter = minFilter;
        }

        if (textureMagFilter != magFilter) {
            glTexParameteri(targetType, GL_TEXTURE_MAG_FILTER, magFilter);
            textureMagFilter = magFilter;
        }

        CheckOpenGLError();
    }
}


void Texture2D::Init2DTexture(unsigned int width, unsigned int height, unsigned int channels)
{
    this->width = width;
    this->height = height;
    this->channels = channels;

    if (textureID)
        glDeleteTextures(1, &textureID);
    glGenTextures(1, &textureID);
    glBindTexture(targetType, textureID);
    SetTextureParameters();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();
}
