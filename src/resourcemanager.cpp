#include "resourcemanager.hpp"
#include "texturemaker.hpp"

void ResourceManager::loadAll()
{
    mTextures.emplace("body", makeTexture("data/textures/body.png"));//0
    mTextures.emplace("head", makeTexture("data/textures/head.png"));//1

    auto texture = [this] (const std::string& name)
    {
        return mTextures.count(name) != 0 ? &mTextures.at(name) : nullptr;
    };

    mAnimations.emplace("body_idle", fea::Animation{{0,0}, {24, 12}, 1, 10}); //body idle 0
    mAnimations.emplace("body_walk", fea::Animation{{0,0}, {24, 24}, 4, 10}); //body walk 1
    mAnimations.emplace("head_aim", fea::Animation{{0,0}, {28, 36}, 5, 1}); //head aim 2

    auto animation = [this] (const std::string& name)
    {
        return mAnimations.count(name) != 0 ? &mAnimations.at(name) : nullptr;
    };

    auto newDisplay = [this] (const std::string& name, RenderDisplay display)
    {
        mDisplayNameIndex.emplace(name, display.displayId);
        mDisplays.emplace_back(std::move(display));
    };

    newDisplay("body_idle", RenderDisplay{0, {-12.0f, 24.0f}, texture("body"), animation("body_idle")});
    newDisplay("body_walk", RenderDisplay{1, {-12.0f, 24.0f}, texture("body"), animation("body_walk")});
    newDisplay("head_aim", RenderDisplay{2, {-4.0f, -8.0f}, texture("head"), animation("head_aim")});
}

const std::vector<RenderDisplay>& ResourceManager::displays() const
{
    return mDisplays;
}

int32_t ResourceManager::displayFromName(const std::string& name) const
{
    return mDisplayNameIndex.at(name);
}
