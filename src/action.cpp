#include "action.hpp"
#include "aim.hpp"
#include "binarysearch.hpp"

void applyActions(const std::vector<Action>& actions, std::vector<Position>& positions, std::vector<PlayerAim>& playerAims, float walkSpeed, float playerAimSpeed)
{
    for(const auto& actionData : actions)
    {
        const ActionType& action = actionData.action;

        auto positionIter = binarySearch(positions.begin(), positions.end(), actionData.objectId, [] (int32_t id, const Position& position)
        {
            return position.objectId == id;
        });

        auto playerAimIter = binarySearch(playerAims.begin(), playerAims.end(), actionData.objectId, [] (int32_t id, const PlayerAim& playerAim)
        {
            return playerAim.objectId == id;
        });

        if(positionIter != positions.end() && playerAimIter != playerAims.end())
        {
            glm::vec2& position = positionIter->position;
            float& playerAim = playerAimIter->aim;

            if(action == ActionType::WALK_RIGHT)
            {
                position.x += walkSpeed;

                if(aimLeft(playerAim))
                    playerAim = turnAim(playerAim);
            }
            if(action == ActionType::WALK_LEFT)
            {
                position.x -= walkSpeed;

                if(!aimLeft(playerAim))
                    playerAim = turnAim(playerAim);
            }
            if(action == ActionType::AIM_UP)
                playerAim = moveAim(playerAim, playerAimSpeed);
            if(action == ActionType::AIM_DOWN)
                playerAim = moveAim(playerAim, -playerAimSpeed);
        }
    }
}
