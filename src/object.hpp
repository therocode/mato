#pragma once
#include <thero/optional.hpp>
#include "data.hpp"

enum ObjectType { PLAYER };

struct Object
{
    th::Optional<Position> position;
    th::Optional<Aim> aim;
    th::Optional<Health> health;
    std::vector<Sprite> sprites;
    th::Optional<AimSpriteInfo> aimSpriteInfo;
    th::Optional<WalkSpriteInfo> walkSpriteInfo;
};
