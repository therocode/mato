#include "action.hpp"
#include "aim.hpp"
#include "binarysearch.hpp"

void applyActions(const std::vector<Action>& actions, std::vector<Position>& positions, std::vector<Aim>& aims, float walkSpeed, float aimSpeed)
{
    for(const auto& actionData : actions)
    {
        const ActionType& action = actionData.action;

        auto positionIter = binarySearch(positions.begin(), positions.end(), actionData.objectId, [] (int32_t id, const Position& position)
        {
            return position.objectId == id;
        });

        auto aimIter = binarySearch(aims.begin(), aims.end(), actionData.objectId, [] (int32_t id, const Aim& aim)
        {
            return aim.objectId == id;
        });

        if(positionIter != positions.end() && aimIter != aims.end())
        {
            glm::vec2& position = positionIter->position;
            float& aim = aimIter->aim;

            if(action == ActionType::WALK_RIGHT)
            {
                position.x += walkSpeed;

                if(aimLeft(aim))
                    aim = turnAim(aim);
            }
            if(action == ActionType::WALK_LEFT)
            {
                position.x -= walkSpeed;

                if(!aimLeft(aim))
                    aim = turnAim(aim);
            }
            if(action == ActionType::AIM_UP)
                aim = moveAim(aim, aimSpeed);
            if(action == ActionType::AIM_DOWN)
                aim = moveAim(aim, -aimSpeed);
        }
    }
}
