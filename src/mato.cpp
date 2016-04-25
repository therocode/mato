#include "pi.hpp"
#include "mato.hpp"
#include "data.hpp"
#include "aim.hpp"
#include "binarysearch.hpp"
#include "texturemaker.hpp"
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>

std::vector<Position> positions;
std::vector<Aim> aims;
std::vector<Health> health;
std::vector<ActionDuration> actionDurations;

PixelMap landscapeForeground;

const float walkSpeed = 4.0f;
const float aimSpeed = pi / 30.0f;

void createInitialData()
{
    positions.emplace_back(
    Position{
            0,
            {275.0f, 384.0f},
    });

    aims.emplace_back(
    Aim{
            0,
            halfPi,
    });

    positions.emplace_back(
    Position{
            1,
            {1105.0f, 384.0f},
    });

    aims.emplace_back(
    Aim{
            1,
            -halfPi,
    });

    landscapeForeground = makePixelMap("data/textures/worldforeground.png");
}

void updateActionDurations(const std::vector<Action>& actions, std::vector<ActionDuration>& durations)
{
    std::vector<int32_t> toErase;

    for(int32_t i = 0; i < static_cast<int32_t>(durations.size()); ++i)
    {
        ActionDuration& duration = durations[i];

        auto actionIter = std::find_if(actions.begin(), actions.end(), [&duration] (const Action& action)
        {
            return duration.id == action.id && duration.action == action.action;
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
            return duration.id == action.id && duration.action == action.action;
        });

        if(durationIter == durations.end())
            durations.emplace_back(ActionDuration{action.id, action.action, 1});
    }
}

void applyActions(const std::vector<Action>& actions)
{
    for(const auto& actionData : actions)
    {
        const ActionType& action = actionData.action;

        auto positionIter = binarySearch(positions.begin(), positions.end(), actionData.id, [] (int32_t id, const Position& position)
        {
            return position.id == id;
        });

        auto aimIter = binarySearch(aims.begin(), aims.end(), actionData.id, [] (int32_t id, const Aim& aim)
        {
            return aim.id == id;
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

    updateActionDurations(actions, actionDurations);
    applyActions(actions);

    mRenderer.startFrame();
    mRenderer.renderWorld(landscapeForeground.pixels(), landscapeForeground.size());
    mRenderer.renderObjects(positions, aims, actionDurations);

    mWindow.swapBuffers();
    mAudioPlayer.update();
}
