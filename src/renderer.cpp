#include "pi.hpp"
#include "renderer.hpp"
#include "texturemaker.hpp"
#include "aim.hpp"

struct AimGraphics
{
    int32_t keyFrame;
    bool flip;
};

AimGraphics toAimGraphics(float aim)
{
    bool flip = aimLeft(aim);

    aim = std::abs(aim);

    int32_t keyFrame = static_cast<int32_t>(aim / (pi / 5.0f));

    return {keyFrame, flip};
}

Renderer::Renderer(fea::Renderer2D& renderer) : mRenderer(renderer),
        mBodyQuad({48.0f, 24.0f}),
        mHeadQuad({56.0f, 72.0f})
{
    mBodyTexture = makeTexture("data/textures/body.png");
    
    mHeadTexture = makeTexture("data/textures/head.png");
    
    mBodyIdle = fea::Animation{{0,0}, {48, 24}, 1, 10};
    mBodyWalking = fea::Animation{{0,0}, {48, 24}, 4, 10};
    mAim = fea::Animation{{0,0}, {56, 72}, 5, 1};

    mBodyQuad.setAnimation(mBodyIdle);
    mHeadQuad.setAnimation(mAim);
}

//temp hack for anim
std::unordered_map<int32_t, int32_t> ticks;

void Renderer::renderObjects(const std::vector<Position>& positions, const std::vector<Aim>& aims, const std::vector<Action>& actions)
{
    auto positionIter = positions.begin();
    auto aimIter = aims.begin();

    mRenderer.clear();

    auto nextValid = [&positionIter, &aimIter, &positions, &aims] ()
    {
        while(positionIter != positions.end() && aimIter != aims.end() && positionIter->id != aimIter->id)
        {
            if(positionIter->id < aimIter->id)
                ++positionIter;
            else if(aimIter->id < positionIter->id)
                ++aimIter;
        }
    };

    nextValid();

    while(positionIter != positions.end() && aimIter != aims.end())
    {
        const glm::vec2& position = positionIter->position;
        float aim = aimIter->aim;

        ticks.emplace(aimIter->id, 0);

        auto actionIter = std::find_if(actions.begin(), actions.end(), [aimIter] (const Action& action)
        {
            return action.id == aimIter->id;
        });

        bool isWalking = actionIter != actions.end() && (actionIter->action == ActionType::WALK_RIGHT || actionIter->action == ActionType::WALK_LEFT);

        AimGraphics aimGraphics = toAimGraphics(aim);
        glm::vec2 bodyOffset = aimGraphics.flip ? glm::vec2(48.0f, 0.0f) : glm::vec2();

        mBodyQuad.setTexture(mBodyTexture);
        mBodyQuad.setOrigin(glm::vec2(42.0f, 0.0f));
        mBodyQuad.setPosition(position + bodyOffset);
        mBodyQuad.setHFlip(aimGraphics.flip);
        mBodyQuad.setAnimation(isWalking ? mBodyWalking : mBodyIdle);
        mBodyQuad.setAnimationFrame(ticks[aimIter->id] / 5 % 4);

        mHeadQuad.setTexture(mHeadTexture);
        mHeadQuad.setAnimationFrame(aimGraphics.keyFrame);
        mHeadQuad.setOrigin(glm::vec2(26.0f, 56.0f));
        mHeadQuad.setPosition(position);
        mHeadQuad.setHFlip(aimGraphics.flip);

        mRenderer.render(mBodyQuad);
        mRenderer.render(mHeadQuad);

        if(isWalking)
            ++ticks[aimIter->id];

        ++positionIter;
        ++aimIter;
        nextValid();
    }
}
