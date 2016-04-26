#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <thero/smartenum.hpp>
#include "pi.hpp"

smart_enum_class(ActionType, WALK_LEFT, WALK_RIGHT, AIM_UP, AIM_DOWN, JUMP, FIRE, WEAPON_CHANGE_LEFT, WEAPON_CHANGE_RIGHT)

struct Action
{
    int32_t id;
    ActionType action;
};

struct ActionDuration
{
    int32_t id;
    ActionType action;
    int32_t duration;
};

struct Position
{
    int32_t id;
    glm::vec2 position;
};

struct Aim
{
    int32_t id;
    float aim;
};

struct Health
{
    int32_t id;
    int32_t health;
};

//rendering data
struct RenderOrder
{
    glm::vec2 position;
    glm::vec2 size;
    int32_t displayId;
    float angle = pi / 2.0f;
    int32_t animationProgress = 0;
};

namespace fea
{
    class Texture;
    class Animation;
}

struct RenderDisplay
{
    int32_t id;
    const fea::Texture* texture;
    const fea::Animation* animation;
};
