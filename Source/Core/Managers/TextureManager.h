#pragma once

#include <unordered_map>
#include <string>
#include <vector>

class Texture2D;

class TextureManager
{
	public:
		static void Init();
		static Texture2D* LoadTexture(const std::string &Path, const char *fileName);
		static void SetTexture(const std::string name, Texture2D * texture);
		static Texture2D* GetTexture(const char* name);
		static Texture2D* GetTexture(unsigned int textureID);

	protected:
		TextureManager() = delete;
		~TextureManager() = delete;

	private:

		static std::unordered_map<std::string, Texture2D*> mapTextures;
		static std::vector<Texture2D*> vTextures;
};