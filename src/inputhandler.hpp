#pragma once
#include <fea/util.hpp>
#include <fea/userinterface.hpp>
#include "data.hpp"
#include "directionresolver.hpp"

class InputHandler
{
    public:
        InputHandler(fea::MessageBus& bus, fea::InputHandler& handler);
        std::vector<Action> process();
    private:
        fea::InputHandler& mHandler;
        fea::MessageBus& mBus;
        DirectionResolver mDirectionResolver1;
        DirectionResolver mDirectionResolver2;
};
