#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <thero/smartenum.hpp>
#include "pi.hpp"

smart_enum_class(ActionType, WALK_LEFT, WALK_RIGHT, AIM_UP, AIM_DOWN, JUMP, FIRE, WEAPON_CHANGE_LEFT, WEAPON_CHANGE_RIGHT)

struct Action
{
    int32_t objectId;
    ActionType action;
};

struct ActionDuration
{
    int32_t objectId;
    ActionType action;
    int32_t duration;
};

struct Position
{
    int32_t objectId;
    glm::vec2 position;
};

struct Aim
{
    int32_t objectId;
    float aim;
};

struct Health
{
    int32_t objectId;
    int32_t health;
};

struct AimDisplayInfo
{
    int32_t objectId;
    int32_t displayId;
    int32_t aimFrameAmount;
};

struct WalkDisplayInfo
{
    int32_t objectId;
    int32_t displayId;
    int32_t walkFrameAmount;
};

//many-to-many relationship between objects and render displays
struct DisplayInstance
{
    int32_t objectId;
    int32_t displayId;
    glm::vec2 size;
    float rotation;
    int32_t animationProgress;
    bool flip;
};

//rendering data
struct RenderOrder
{
    glm::vec2 position;
    glm::vec2 size;
    int32_t displayId;
    float rotation = pi / 2.0f;
    int32_t animationProgress = 0;
    bool flip;
};

namespace fea
{
    class Texture;
    class Animation;
}

struct RenderDisplay
{
    int32_t displayId;
    glm::vec2 offset;
    const fea::Texture* texture;
    const fea::Animation* animation;
};
