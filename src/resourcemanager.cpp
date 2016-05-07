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
    mAnimations.emplace("body_walk", fea::Animation{{0,0}, {24, 12}, 4, 10}); //body walk 1
    mAnimations.emplace("head_aim", fea::Animation{{0,0}, {28, 36}, 5, 1}); //head aim 2

    auto animation = [this] (const std::string& name)
    {
        return mAnimations.count(name) != 0 ? &mAnimations.at(name) : nullptr;
    };

    auto newAppearance = [this] (const std::string& name, Appearance appearance)
    {
        mAppearanceNameIndex.emplace(name, appearance.appearanceId);
        mAppearances.emplace_back(std::move(appearance));
    };

    newAppearance("body_idle", Appearance{0, {-12.0f, 24.0f}, texture("body"), animation("body_idle")});
    newAppearance("body_walk", Appearance{1, {-12.0f, 20.0f}, texture("body"), animation("body_walk")});
    newAppearance("head_aim", Appearance{2, {-4.0f, -8.0f}, texture("head"), animation("head_aim")});
}

const std::vector<Appearance>& ResourceManager::appearances() const
{
    return mAppearances;
}

int32_t ResourceManager::appearanceFromName(const std::string& name) const
{
    return mAppearanceNameIndex.at(name);
}
