#include "landscapecollision.hpp"
#include "vectorutil.hpp"

std::vector<LandscapeCollision> checkLandscapeCollisions(const std::vector<Position>& positions, const std::vector<CollisionBox>& collisionBoxes, const CollisionMap& collisionMap)
{
    std::vector<LandscapeCollision> result;

    for(const auto& collisionBox : collisionBoxes)
    {
        const auto* position = findIf(positions, [&collisionBox] (const Position& entry)
        {
            return entry.objectId == collisionBox.objectId;
        });

        if(position)
        {
            glm::ivec2 landscapeCoordinate = position->position / 4.0f;

            if(collisionMap.at(landscapeCoordinate) != WallType::AIR)
            {
                result.emplace_back(LandscapeCollision
                {
                    position->objectId,
                    collisionMap.at(landscapeCoordinate),
                    landscapeCoordinate,
                    position->position,
                });
            }
        }
    }

    return result;
}
