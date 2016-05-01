#include "pi.hpp"
#include "mato.hpp"
#include "data.hpp"
#include "aim.hpp"
#include "binarysearch.hpp"
#include "texturemaker.hpp"
#include "createobject.hpp"
#include "vectorutil.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>

const float walkSpeed = 4.0f;
const float aimSpeed = pi / 30.0f;

void Mato::addObject(Object object)
{
    emplaceOptional(std::move(object.position), mPositions);
    emplaceOptional(std::move(object.health), mHealth);
    emplaceOptional(std::move(object.aim), mAims);

    for(auto& displayInstance : object.displays)
        mDisplays.emplace_back(std::move(displayInstance));
}

void Mato::loadResources()
{
}

void Mato::createInitialData()
{
    addObject(createObject(0, PLAYER, {275.0f, 384.0f}, mResources));
    addObject(createObject(1, PLAYER, {1105.0f, 384.0f}, mResources));

    mLandscapeForeground = makePixelMap("data/textures/worldforeground.png");
}

void updateActionDurations(const std::vector<Action>& actions, std::vector<ActionDuration>& durations)
{
    std::vector<int32_t> toErase;

    for(int32_t i = 0; i < static_cast<int32_t>(durations.size()); ++i)
    {
        ActionDuration& duration = durations[i];

        auto actionIter = std::find_if(actions.begin(), actions.end(), [&duration] (const Action& action)
        {
            return duration.objectId == action.objectId && duration.action == action.action;
        });

        if(actionIter != actions.end())
        {
            ++duration.duration;
        }
        else
        {
            toErase.emplace_back(i);
        }
    }

    std::reverse(toErase.begin(), toErase.end());

    for(auto index : toErase)
    {
        durations.erase(durations.begin() + index);
    }

    for(int32_t i = 0; i < static_cast<int32_t>(actions.size()); ++i)
    {
        const Action& action = actions[i];

        auto durationIter = std::find_if(durations.begin(), durations.end(), [&action] (const ActionDuration& duration)
        {
            return duration.objectId == action.objectId && duration.action == action.action;
        });

        if(durationIter == durations.end())
            durations.emplace_back(ActionDuration{action.objectId, action.action, 1});
    }
}

void applyActions(const std::vector<Action>& actions, std::vector<Position>& positions, std::vector<Aim>& aims)
{
    for(const auto& actionData : actions)
    {
        const ActionType& action = actionData.action;

        auto positionIter = binarySearch(positions.begin(), positions.end(), actionData.objectId, [] (int32_t id, const Position& position)
        {
            return position.objectId == id;
        });

        auto aimIter = binarySearch(aims.begin(), aims.end(), actionData.objectId, [] (int32_t id, const Aim& aim)
        {
            return aim.objectId == id;
        });

        if(positionIter != positions.end() && aimIter != aims.end())
        {
            glm::vec2& position = positionIter->position;
            float& aim = aimIter->aim;

            if(action == ActionType::WALK_RIGHT)
            {
                position.x += walkSpeed;

                if(aimLeft(aim))
                    aim = turnAim(aim);
            }
            if(action == ActionType::WALK_LEFT)
            {
                position.x -= walkSpeed;

                if(!aimLeft(aim))
                    aim = turnAim(aim);
            }
            if(action == ActionType::AIM_UP)
                aim = moveAim(aim, aimSpeed);
            if(action == ActionType::AIM_DOWN)
                aim = moveAim(aim, -aimSpeed);
        }
    }
}

Mato::Mato() :
    mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(1366, 768), "Mato"),
    mFeaRenderer(fea::Viewport({1366, 768}, {0, 0}, fea::Camera({1366 / 2.0f, 768 / 2.0f}))),
    mFeaInputHandler(new fea::SDL2InputBackend()),
    mInputHandler(mBus, mFeaInputHandler),
    mAudioPlayer(mBus),
    mRenderer(mFeaRenderer)
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

void Mato::loop()
{
    std::vector<Action> actions = mInputHandler.process();

    updateActionDurations(actions, mActionDurations);
    applyActions(actions, mPositions, mAims);

    mRenderer.startFrame();
    mRenderer.renderWorld(mLandscapeForeground.pixels(), mLandscapeForeground.size());
    mRenderer.renderObjects(mPositions, mAims, mActionDurations);

    mWindow.swapBuffers();
    mAudioPlayer.update();
}
