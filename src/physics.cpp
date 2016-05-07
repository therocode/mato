#include "physics.hpp"
#include "vectorutil.hpp"

void updateVelocities(std::vector<Physics>& physicses, float gravityConstant)
{
    for(auto& physics : physicses)
    {
        physics.velocity += physics.acceleration + glm::vec2(0.0f, gravityConstant);
    }
}

void updatePositions(const std::vector<Physics>& physicses, std::vector<Position>& positions)
{
    for(auto& physics : physicses)
    {
        Position* position = findIf(positions, [&physics] (const Position& entry)
        {
            return entry.objectId == physics.objectId;
        });

        if(position)
            position->position += physics.velocity;
    }
}
