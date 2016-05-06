#pragma once
#include <thero/optional.hpp>
#include "data.hpp"

enum ObjectType { PLAYER };

struct Object
{
    th::Optional<Position> position;
    th::Optional<PlayerAim> playerAim;
    th::Optional<Health> health;
    std::vector<DisplayInstance> displays;
};
