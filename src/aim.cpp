#include "aim.hpp"
#include "pi.hpp"
#include <algorithm>

bool aimLeft(float angle)
{
    return angle < 0.0f;
}

float moveAim(float startAim, float amount)
{
    const float capper = 0.001f; //needed for precision stuff

    if(aimLeft(startAim))
    {//It aims to the left, so we need to invert amount so that "up" is correct
        return std::min(0.0f - capper, std::max(-pi + capper, startAim - amount));
    }
    else
    {
        return std::max(0.0f + capper, std::min(pi - capper, startAim + amount));
    }
}

float turnAim(float startAim)
{
    return -startAim;
}
