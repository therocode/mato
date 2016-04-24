#pragma once
#include <fea/render2d.hpp>
#include "data.hpp"

class Renderer
{
    public:
        Renderer(fea::Renderer2D& renderer);
        void renderObjects(const std::vector<Position>& positions, const std::vector<Aim>& aims, const std::vector<Action>& actions);
    private:
        fea::Renderer2D& mRenderer;
        fea::Texture mBodyTexture;
        fea::Texture mHeadTexture;
        fea::Animation mBodyIdle;
        fea::Animation mBodyWalking;
        fea::Animation mAim;
        fea::AnimatedQuad mBodyQuad;
        fea::AnimatedQuad mHeadQuad;
};
