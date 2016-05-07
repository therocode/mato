#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fea/render2d.hpp>
#include "data.hpp"

class ResourceManager
{
    public:
        void loadAll();
        const std::vector<Appearance>& appearances() const;
        int32_t appearanceFromName(const std::string& name) const;
    private:
        std::unordered_map<std::string, fea::Texture> mTextures;
        std::unordered_map<std::string, fea::Animation> mAnimations;
        std::vector<Appearance> mAppearances;
        std::unordered_map<std::string, int32_t> mAppearanceNameIndex;
};
