#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "core/gpu/texture2D.h"


class TextureManager
{
 public:
    static void Init(const std::string &selfDir);
    static Texture2D *LoadTexture(const std::string &Path, const char *fileName, const char *key = nullptr, bool forceLoad = false, bool cacheInRAM = false);
    static void SetTexture(const std::string name, Texture2D * texture);
    static Texture2D* GetTexture(const char* name);
    static Texture2D* GetTexture(unsigned int textureID);

 protected:
    TextureManager() = delete;
    ~TextureManager() = delete;

 private:
    static std::unordered_map<std::string, Texture2D*> mapTextures;
    static std::vector<Texture2D*> vTextures;
    static std::string selfDir;
};
