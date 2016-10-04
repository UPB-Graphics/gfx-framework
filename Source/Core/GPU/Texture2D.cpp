#include "Texture2D.h"

#include <thread>
#include <iostream>

#include <include/gl.h>

using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

using uint = unsigned int;
using uchar = unsigned char;

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

void write_image_thread(const char* fileName, uint width, uint height, uint channels, const uchar *data)
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

bool Texture2D::Load2D(const char* fileName, GLenum wrapping_mode)
{
	int width, height, chn;
	unsigned char *data = stbi_load(fileName, &width, &height, &chn, 0);

	if (data == NULL) {
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
	glTexImage2D(targetType, 0, internalFormat[0][chn], width, height, 0, pixelFormat[chn], GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(targetType);
	glBindTexture(targetType, 0);
	CheckOpenGLError();

	stbi_image_free(data);
	return true;
}

void Texture2D::SaveToFile(const char * fileName) const
{
	unsigned char *data = new unsigned char[width * height * channels];
	glBindTexture(targetType, textureID);
	glGetTexImage(targetType, 0, pixelFormat[channels], GL_UNSIGNED_BYTE, (void*)data);

	stbi_write_png(fileName, width, height, channels, data, width * channels);
	SAFE_FREE_ARRAY(data);
}

void Texture2D::UploadNewData(const uchar *img)
{
	Bind();
	glTexSubImage2D(targetType, 0, 0, 0, width, height, pixelFormat[channels], GL_UNSIGNED_BYTE, img);
	UnBind();
}

void Texture2D::UploadNewData(const ushort *img)
{
	Bind();
	glTexSubImage2D(targetType, 0, 0, 0, width, height, pixelFormat[channels], GL_UNSIGNED_SHORT, img);
	UnBind();
}

void Texture2D::Create(const unsigned char* img, int width, int height, int chn)
{
	Init2DTexture(width, height, chn);
	glTexImage2D(targetType, 0, internalFormat[0][chn], width, height, 0, pixelFormat[chn], GL_UNSIGNED_BYTE, (void*)img);
	UnBind();
}

void Texture2D::CreateU16(const unsigned short* img, int width, int height, int chn)
{
	Init2DTexture(width, height, chn);
	glTexImage2D(targetType, 0, internalFormat[1][chn], width, height, 0, pixelFormat[chn], GL_UNSIGNED_SHORT, (void*)img);
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
	glTexParameterf(targetType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
}

unsigned int Texture2D::GetWidth() const
{
	return width;
}

unsigned int Texture2D::GetHeight() const
{
	return height;
}

void Texture2D::GetSize(unsigned int &width,unsigned int &height) const
{
	width = this->width;
	height = this->height;
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
