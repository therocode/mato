#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <thero/smartenum.hpp>

smart_enum_class(ActionType, WALK_LEFT, WALK_RIGHT, AIM_UP, AIM_DOWN, JUMP, FIRE, WEAPON_CHANGE_LEFT, WEAPON_CHANGE_RIGHT)

struct Action
{
    int32_t id;
    ActionType action;
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
