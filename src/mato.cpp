#include "pi.hpp"
#include "mato.hpp"
#include "data.hpp"
#include "aim.hpp"
#include "texturemaker.hpp"
#include "createobject.hpp"
#include "vectorutil.hpp"
#include "action.hpp"
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
    mRenderer(mFeaRenderer, mResources.displays())
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
    emplaceOptional(std::move(object.health), mHealth);
    emplaceOptional(std::move(object.playerAim), mPlayerAims);

    for(auto& displayInstance : object.displays)
        mDisplays.emplace_back(std::move(displayInstance));
}

void Mato::loop()
{
    std::vector<Action> actions = mInputHandler.process();

    updateActionDurations(actions);
    applyActions(actions, mPositions, mPlayerAims, walkSpeed, aimSpeed);

    mRenderer.startFrame();
    mRenderer.renderWorld(mLandscapeForeground.pixels(), mLandscapeForeground.size());
    renderDisplays();

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

    for(int32_t i = 0; i < static_cast<int32_t>(mActionDurations.size()); ++i)
    {
        ActionDuration& duration = mActionDurations[i];

        auto actionIter = std::find_if(actions.begin(), actions.end(), [&duration] (const Action& action)
        {
            return duration.objectId == action.objectId && duration.action == action.action;
        });

        if(actionIter != actions.end())
        {
            ++duration.duration;
            newDurations.push_back(duration);
        }
    }

    mActionDurations = newDurations;
    mActionDurations.shrink_to_fit();
}

void Mato::renderDisplays()
{
    std::vector<RenderOrder> orders;

    for(const auto& display : mDisplays)
    {
        auto idFinder = [&display] (const auto& entry) { return entry.objectId == display.objectId; };

        const Position* position = findIf(mPositions, idFinder); TH_ASSERT(position, "Something had a display but no position");

        orders.emplace_back(
            RenderOrder{
                position->position,
                display.size,
                display.displayId,
                display.rotation,
                display.animationProgress,
                display.flip,
            }
        );
    }

    mRenderer.render(orders);
}
