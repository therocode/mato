#pragma once
#include <vector>
#include "data.hpp"
#include "walltype.hpp"
#include "collisionmap.hpp"

struct LandscapeCollision
{
    int32_t objectId;
    WallType wallType;
    glm::ivec2 collisionPoint;
    glm::vec2 relativeCollisionPoint;
};

std::vector<LandscapeCollision> checkLandscapeCollisions(const std::vector<Position>& positions, const std::vector<CollisionBox>& collisionBoxes, const CollisionMap& collisionMap);
