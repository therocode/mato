#include "pi.hpp"
#include "mato.hpp"
#include "data.hpp"
#include "aim.hpp"
#include "texturemaker.hpp"
#include "createobject.hpp"
#include "vectorutil.hpp"
#include "action.hpp"
#include "aimgraphics.hpp"
#include "physics.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>

const float walkSpeed = 4.0f;
const float aimSpeed = pi / 30.0f;

Mato::Mato() :
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(1366, 768), "Mato"),
    mFeaRenderer(fea::Viewport({1366, 768}, {0, 0}, fea::Camera({1366 / 2.0f, 768 / 2.0f}))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInputHandler(mBus, mFeaInputHandler),
    mAudioPlayer(mBus),
    mRenderer(mFeaRenderer, mResources.appearances())
{
    mWindow.setVSyncEnabled(true);
    mWindow.setFramerateLimit(60);

    subscribe(mBus, *this, false);

    mResources.loadAll();

    createInitialData();
}

void Mato::handleMessage(const QuitMessage& message)
{
    (void)message;
    quit();
}

void Mato::addObject(Object object)
{
    emplaceOptional(std::move(object.position), mPositions);
    emplaceOptional(std::move(object.physics), mPhysics);
    emplaceOptional(std::move(object.health), mHealth);
    emplaceOptional(std::move(object.aim), mAims);

    for(auto& sprite : object.sprites)
        mSprites.emplace_back(std::move(sprite));

    emplaceOptional(std::move(object.aimSpriteInfo), mAimSpriteInfos);
    emplaceOptional(std::move(object.walkSpriteInfo), mWalkSpriteInfos);
}

void Mato::loop()
{
    std::vector<Action> actions = mInputHandler.process();

    updateActionDurations(actions);
    applyActions(actions, mPositions, mAims, walkSpeed, aimSpeed);

    updateAimSprites();
    updateWalkSprites();

    updateVelocities(mPhysics, 0.5f);
    updatePositions(mPhysics, mPositions);

    mRenderer.startFrame();
    mRenderer.renderWorld(mLandscapeForeground.pixels(), mLandscapeForeground.size());
    renderSprites();

    mWindow.swapBuffers();
    mAudioPlayer.update();
}

void Mato::createInitialData()
{
    addObject(createObject(0, PLAYER, {275.0f, 384.0f}, mResources));
    addObject(createObject(1, PLAYER, {1105.0f, 384.0f}, mResources));

    mLandscapeForeground = makePixelMap("data/textures/worldforeground.png");
}

void Mato::updateActionDurations(const std::vector<Action>& actions)
{
    std::vector<ActionDuration> newDurations;
    newDurations.reserve(mActionDurations.size());

    for(int32_t i = 0; i < static_cast<int32_t>(actions.size()); ++i)
    {
        const Action& action = actions[i];

        auto actionDurationIter = std::find_if(mActionDurations.begin(), mActionDurations.end(), [&action] (const ActionDuration& duration)
        {
            return duration.objectId == action.objectId && duration.action == action.action;
        });

        if(actionDurationIter != mActionDurations.end())
        {
            ActionDuration duration = *actionDurationIter;
            ++duration.duration;
            newDurations.push_back(duration);
        }
        else
        {
            newDurations.push_back(ActionDuration{action.objectId, action.action, 1});
        }
    }

    mActionDurations = newDurations;
    mActionDurations.shrink_to_fit();
}

void Mato::updateAimSprites()
{
    for(const auto& aimSpriteInfo : mAimSpriteInfos)
    {
        int32_t objectId = aimSpriteInfo.objectId;
        int32_t appearanceId = aimSpriteInfo.appearanceId;

        Sprite* spriteToUpdate = findIf(mSprites, [objectId, appearanceId] (const Sprite& entry)
        {
            return entry.objectId == objectId && entry.appearanceId == appearanceId;
        });

        TH_ASSERT(spriteToUpdate, "There is aim sprite info for object: " << objectId << " with appearance " << appearanceId << " but there is no such sprite instance");

        const Appearance* appearance = findIf(mResources.appearances(), [appearanceId] (const Appearance& entry)
        {
            return entry.appearanceId == appearanceId;
        });

        const Aim* aim = findIf(mAims, [objectId] (const Aim& entry)
        {
            return objectId == entry.objectId;
        });

        TH_ASSERT(aim, "There is aim sprite info for object: " << objectId << " but there is no aim for it");

        auto aimGraphics = toAimGraphics(aim->aim, appearance->animation->getFrameAmount());
        
        spriteToUpdate->flip = aimGraphics.flip;
        spriteToUpdate->animationProgress = aimGraphics.keyFrame;       
    }
}

void Mato::updateWalkSprites()
{
    for(const auto& walkSpriteInfo : mWalkSpriteInfos)
    {
        int32_t objectId = walkSpriteInfo.objectId;
        int32_t appearanceId = walkSpriteInfo.appearanceId;

        Sprite* spriteToUpdate = findIf(mSprites, [objectId, appearanceId] (const Sprite& entry)
        {
            return entry.objectId == objectId && entry.appearanceId == appearanceId;
        });

        TH_ASSERT(spriteToUpdate, "There is walk sprite info for object: " << objectId << " with appearance " << appearanceId << " but there is no such sprite instance");

        const ActionDuration* actionDuration = findIf(mActionDurations, [objectId] (const ActionDuration& entry)
        {
            return objectId == entry.objectId && (entry.action == ActionType::WALK_LEFT || entry.action == ActionType::WALK_RIGHT);
        });

        if(actionDuration)
        {
            spriteToUpdate->flip = actionDuration->action == ActionType::WALK_LEFT;
            spriteToUpdate->animationProgress = actionDuration->duration;
        }
    }
}

void Mato::renderSprites()
{
    std::vector<RenderOrder> orders;

    for(const auto& sprite : mSprites)
    {
        auto idFinder = [&sprite] (const auto& entry) { return entry.objectId == sprite.objectId; };

        const Position* position = findIf(mPositions, idFinder); TH_ASSERT(position, "Entry with Display ID " << sprite.appearanceId << " had a sprite but no position");

        orders.emplace_back(
            RenderOrder{
                position->position,
                sprite.size,
                sprite.appearanceId,
                sprite.rotation,
                sprite.animationProgress,
                sprite.flip,
            }
        );
    }

    mRenderer.render(orders);
}
