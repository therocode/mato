#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <fea/rendering/color.hpp>
#include "walltype.hpp"

class CollisionMap
{
    public:
        CollisionMap() = default;
        CollisionMap(std::vector<WallType> collisionValues, glm::ivec2 size);
        const WallType* collisionValues() const;
        const glm::ivec2& size() const;
        WallType at(const glm::ivec2& coordinate) const;
    private:
        std::vector<WallType> mCollisionValues;
        glm::ivec2 mSize;
};

CollisionMap makeCollisionMap(const std::string& path);
WallType colorToCollisionType(const fea::Color& color);
