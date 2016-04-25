#pragma once
#include <fea/render2d.hpp>
#include "data.hpp"

class Renderer
{
    public:
        Renderer(fea::Renderer2D& renderer);
        void startFrame();
        void renderWorld(const uint8_t* foregroundPixels, const glm::ivec2& foregroundSize);
        void renderObjects(const std::vector<Position>& positions, const std::vector<Aim>& aims, const std::vector<ActionDuration>& actions);
    private:
        fea::Renderer2D& mRenderer;
        
        fea::Texture mLandscapeMap;

        fea::Texture mBodyTexture;
        fea::Texture mHeadTexture;
        fea::Animation mBodyIdle;
        fea::Animation mBodyWalking;
        fea::Animation mAim;
        fea::AnimatedQuad mBodyQuad;
        fea::AnimatedQuad mHeadQuad;
};
