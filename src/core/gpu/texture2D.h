#pragma once

#include "utils/gl_utils.h"


class Texture2D
{
 public:
    Texture2D();
    ~Texture2D();

    void Bind() const;
    void BindToTextureUnit(GLenum TextureUnit) const;
    void UnBind() const;

    void UploadNewData(const unsigned char *img);
    void UploadNewData(const unsigned int *img);

    void Init(GLuint gpuTextureID, unsigned int width, unsigned int height, unsigned int channels);
    void Create(const unsigned char* img, int width, int height, int chn);
    void CreateU16(const unsigned int* img, int width, int height, int chn);

    void CreateCubeTexture(const float *data, unsigned int width, unsigned int height, unsigned int chn);
    void CreateFrameBufferTexture(unsigned int width, unsigned int height, unsigned int targetID, unsigned int precision = 32);
    void CreateDepthBufferTexture(unsigned int width, unsigned int height);

    bool Load2D(const char* fileName, GLenum wrappingMode = GL_REPEAT);
    void SaveToFile(const char* fileName);
    void CacheInMemory(bool state);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    void GetSize(unsigned int &width, unsigned int &height) const;
    unsigned char *GetImageData() const;

    unsigned int GetNrChannels() const;

    void SetWrappingMode(GLenum mode);
    void SetFiltering(GLenum minFilter, GLenum magFilter = GL_LINEAR);

    GLuint GetTextureID() const;

 private:
    void SetTextureParameters();
    void Init2DTexture(unsigned int width, unsigned int height, unsigned int channels);

 private:
    bool cacheInMemory;
    unsigned int bitsPerPixel;
    unsigned int width;
    unsigned int height;
    unsigned int channels;

    GLuint targetType;
    GLuint textureID;
    GLenum wrappingMode;
    GLenum textureMinFilter;
    GLenum textureMagFilter;

    unsigned char *imageData;
};
