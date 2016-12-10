#pragma once
#include <include/gl.h>
#include <include/utils.h>

class Texture2D
{
	public:
		Texture2D();
		~Texture2D();

		void Bind() const;
		void BindToTextureUnit(GLenum TextureUnit) const;
		void UnBind() const;

		void UploadNewData(const uchar *img);
		void UploadNewData(const ushort *img);

		void Init(GLuint gpuTextureID, unsigned int width, unsigned int height, unsigned int channels);
		void Create(const unsigned char* img, int width, int height, int chn);
		void CreateU16(const unsigned short* img, int width, int height, int chn);

		bool Load2D(const char* fileName, GLenum wrappingMode = GL_REPEAT);
		void SaveToFile(const char* fileName) const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		void GetSize(unsigned int &width, unsigned int &height) const;

		void SetWrappingMode(GLenum mode);
		void SetFiltering(GLenum minFilter, GLenum magFilter = GL_LINEAR);

		GLuint GetTextureID() const;

	private:
		void SetTextureParameters();
		void Init2DTexture(unsigned int width, unsigned int height, unsigned int channels);

	private:
		unsigned int width;
		unsigned int height;
		unsigned int channels;
		GLuint targetType;
		GLuint textureID;
		GLenum wrappingMode;
		GLenum textureMinFilter;
		GLenum textureMagFilter;
};
