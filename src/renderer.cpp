#include "pi.hpp"
#include "renderer.hpp"
#include "texturemaker.hpp"
#include "vectorutil.hpp"

Renderer::Renderer(fea::Renderer2D& renderer, const std::vector<RenderDisplay>& displays):
    mRenderer(renderer),
    mDisplays(displays)
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
        const RenderDisplay* display = findIf(mDisplays, [&renderOrder] (const RenderDisplay& disp) { return disp.displayId == renderOrder.displayId; });
        TH_ASSERT(display, "render order uses invalid display " << renderOrder.displayId);

        fea::AnimatedQuad quad(renderOrder.size);

        if(display->texture)
            quad.setTexture(*display->texture);
        if(display->animation)
            quad.setAnimation(*display->animation);

        quad.setAnimationFrame(renderOrder.animationProgress / display->animation->getDelay() % display->animation->getFrameAmount());

        auto offset = display->offset;
        if(renderOrder.flip)
            offset.x *= -1.0f;

        quad.setPosition(renderOrder.position + offset);
        quad.setRotation(renderOrder.rotation - pi / 2.0f); //take away pi/2.0f which is a quarter of a full turn because featherkit expects 0 degrees to be --> but in the game, it is downwards
        quad.setHFlip(renderOrder.flip);

        mRenderer.render(quad);
    }
}
