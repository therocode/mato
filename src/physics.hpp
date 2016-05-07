#pragma once
#include "data.hpp"

void updateVelocities(std::vector<Physics>& physicses, float gravityConstant);
void updatePositions(const std::vector<Physics>& physicses, std::vector<Position>& positions);
