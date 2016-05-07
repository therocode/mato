#include "pi.hpp"
#include "renderer.hpp"
#include "texturemaker.hpp"
#include "vectorutil.hpp"

Renderer::Renderer(fea::Renderer2D& renderer, const std::vector<Appearance>& appearances):
    mRenderer(renderer),
    mAppearances(appearances)
{
}

void Renderer::startFrame()
{
    mRenderer.clear();
}

void Renderer::renderWorld(const uint8_t* foregroundPixels, const glm::ivec2& size)
{
    mLandscapeMap.create(size, foregroundPixels);

    fea::Quad landscapeQuad(size * 4);
    landscapeQuad.setTexture(mLandscapeMap);
    
    mRenderer.render(landscapeQuad);
}

void Renderer::render(const std::vector<RenderOrder>& orders) const
{
    for(const auto& renderOrder : orders)
    {
        const Appearance* appearance = findIf(mAppearances, [&renderOrder] (const Appearance& disp) { return disp.appearanceId == renderOrder.appearanceId; });
        TH_ASSERT(appearance, "render order uses invalid appearance " << renderOrder.appearanceId);

        fea::AnimatedQuad quad(renderOrder.size);

        if(appearance->texture)
            quad.setTexture(*appearance->texture);
        if(appearance->animation)
            quad.setAnimation(*appearance->animation);

        quad.setAnimationFrame(renderOrder.animationProgress / appearance->animation->getDelay() % appearance->animation->getFrameAmount());

        auto offset = appearance->offset;
        if(renderOrder.flip)
            offset.x *= -1.0f;

        quad.setPosition(renderOrder.position + offset);
        quad.setRotation(renderOrder.rotation - pi / 2.0f); //take away pi/2.0f which is a quarter of a full turn because featherkit expects 0 degrees to be --> but in the game, it is downwards
        quad.setHFlip(renderOrder.flip);

        mRenderer.render(quad);
    }
}
