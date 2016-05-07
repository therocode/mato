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

//physics data

struct Position
{
    int32_t objectId;
    glm::vec2 position;
};

struct Physics
{
    int32_t objectId;
    glm::vec2 velocity;
    glm::vec2 acceleration;
};

struct CollisionBox
{
    int32_t objectId;
    glm::vec2 start;
    glm::vec2 size;
};

struct BottomRestPercent
{
    int32_t objectId;
    float bottomRestPercent;
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

struct AimSpriteInfo
{
    int32_t objectId;
    int32_t appearanceId;
};

struct WalkSpriteInfo
{
    int32_t objectId;
    int32_t appearanceId;
};

//rendering data
//many-to-many relationship between objects and appearances
struct Sprite
{
    int32_t objectId;
    int32_t appearanceId;
    glm::vec2 size;
    float rotation;
    int32_t animationProgress;
    bool flip;
};

struct RenderOrder
{
    glm::vec2 position;
    glm::vec2 size;
    int32_t appearanceId;
    float rotation = pi / 2.0f;
    int32_t animationProgress = 0;
    bool flip;
};

namespace fea
{
    class Texture;
    class Animation;
}

struct Appearance
{
    int32_t appearanceId;
    glm::vec2 offset;
    const fea::Texture* texture;
    const fea::Animation* animation;
};
