#pragma once
#include <cstdint>
#include <cmath>

struct AimGraphics
{
    int32_t keyFrame;
    bool flip;
};

AimGraphics toAimGraphics(float aim, int32_t frameAmount);
