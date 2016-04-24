#include "directionresolver.hpp"

void DirectionResolver::upActive(bool b)
{
    up = b;
}

void DirectionResolver::downActive(bool b)
{
    down = b;
}

void DirectionResolver::leftActive(bool b)
{
    left = b;
}

void DirectionResolver::rightActive(bool b)
{
    right = b;
}

glm::vec2 DirectionResolver::getDirection()
{
    glm::vec2 result;

    if(up)
        result.y += -1.0f;
    if(down)
        result.y += 1.0f;
    if(left)
        result.x += -1.0f;
    if(right)
        result.x += 1.0f;

    if(glm::length(result) > 0.0f)
        result = glm::normalize(result);

    return result;
}
