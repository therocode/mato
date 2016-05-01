#pragma once
#include "object.hpp"

Object createObject(int32_t id, ObjectType type, glm::vec2 position, const ResourceManager& resources)
{
    Object result;

    result.position = Position{id, std::move(position)};

    if(type == PLAYER)
    {
        result.aim = Aim{id, pi/2.0f};
        result.health = Health{id, 100};
        result.displays = {DisplayInstance{id, resources.displayFromName("body_idle")}, DisplayInstance{id, resources.displayFromName("head_aim")}};
    }

    return result;
}
