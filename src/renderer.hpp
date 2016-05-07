#pragma once
#include <fea/render2d.hpp>
#include "data.hpp"

class Renderer
{
    public:
        Renderer(fea::Renderer2D& renderer, const std::vector<Appearance>& appearances);
        void startFrame();
        void renderWorld(const uint8_t* foregroundPixels, const glm::ivec2& foregroundSize);
        void render(const std::vector<RenderOrder>& orders) const;
    private:
        fea::Renderer2D& mRenderer;
        
        const std::vector<Appearance>& mAppearances;
        fea::Texture mLandscapeMap;
};
