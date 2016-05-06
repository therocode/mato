#pragma once
#include "object.hpp"

Object createObject(int32_t id, ObjectType type, glm::vec2 position, const ResourceManager& resources)
{
    Object result;

    result.position = Position{id, std::move(position)};

    if(type == PLAYER)
    {
        result.playerAim = PlayerAim{id, pi/2.0f};
        result.health = Health{id, 100};
        result.displays =
        {
            //head
            DisplayInstance
            {
                id,
                resources.displayFromName("head_aim"),
                {28.0f, 36.0f}, //size
                pi / 2.0f,
                0,
                false,
            },
            //body
            DisplayInstance
            {
                id,
                resources.displayFromName("body_idle"),
                {24.0f, 12.0f}, //size
                pi / 2.0f,
                0,
                false,
            },
        };
    }

    return result;
}
