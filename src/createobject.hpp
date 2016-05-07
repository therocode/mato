#pragma once
#include "object.hpp"

Object createObject(int32_t id, ObjectType type, glm::vec2 position, const ResourceManager& resources)
{
    Object result;

    result.position = Position{id, std::move(position)};

    if(type == PLAYER)
    {
        result.physics = Physics
        {
            id,
            {},
            {},
        };
        result.aim = Aim{id, pi/2.0f};
        result.health = Health{id, 100};
        result.sprites =
        {
            //body
            Sprite
            {
                id,
                resources.appearanceFromName("body_walk"),
                {24.0f, 12.0f}, //size
                pi / 2.0f,
                0,
                false,
            },
            //head
            Sprite
            {
                id,
                resources.appearanceFromName("head_aim"),
                {28.0f, 36.0f}, //size
                pi / 2.0f,
                0,
                false,
            },
        };
        result.walkSpriteInfo = WalkSpriteInfo
        {
            id,
            resources.appearanceFromName("body_walk"),
        };
        result.aimSpriteInfo = AimSpriteInfo
        {
            id,
            resources.appearanceFromName("head_aim"),
        };
    }

    return result;
}
