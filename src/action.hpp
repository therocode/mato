#pragma once
#include <vector>
#include "data.hpp"

void applyActions(const std::vector<Action>& actions, std::vector<Position>& positions, std::vector<PlayerAim>& aims, float walkSpeed, float aimSpeed);
