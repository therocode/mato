#include "inputhandler.hpp"
#include "messages.hpp"

InputHandler::InputHandler(fea::MessageBus& bus, fea::InputHandler& handler):
    mHandler(handler),
    mBus(bus)
{
}

std::vector<Action> InputHandler::process()
{
    std::vector<Action> actions;

    fea::Event event;
    while(mHandler.pollEvent(event))
    {
        if(event.type == fea::Event::KEYPRESSED)
        {
            if(event.key.code == fea::Keyboard::ESCAPE)
                mBus.send(QuitMessage());
            else if(event.key.code == fea::Keyboard::W)
                mDirectionResolver1.upActive(true);
            else if(event.key.code == fea::Keyboard::S)
                mDirectionResolver1.downActive(true);
            else if(event.key.code == fea::Keyboard::A)
                mDirectionResolver1.leftActive(true);
            else if(event.key.code == fea::Keyboard::D)
                mDirectionResolver1.rightActive(true);
            else if(event.key.code == fea::Keyboard::I)
                mDirectionResolver2.upActive(true);
            else if(event.key.code == fea::Keyboard::K)
                mDirectionResolver2.downActive(true);
            else if(event.key.code == fea::Keyboard::J)
                mDirectionResolver2.leftActive(true);
            else if(event.key.code == fea::Keyboard::L)
                mDirectionResolver2.rightActive(true);

            mBus.send(KeyPressedMessage{event.key.code});
        }
        if(event.type == fea::Event::KEYRELEASED)
        {
            if(event.key.code == fea::Keyboard::W)
                mDirectionResolver1.upActive(false);
            else if(event.key.code == fea::Keyboard::S)
                mDirectionResolver1.downActive(false);
            else if(event.key.code == fea::Keyboard::A)
                mDirectionResolver1.leftActive(false);
            else if(event.key.code == fea::Keyboard::D)
                mDirectionResolver1.rightActive(false);
            else if(event.key.code == fea::Keyboard::I)
                mDirectionResolver2.upActive(false);
            else if(event.key.code == fea::Keyboard::K)
                mDirectionResolver2.downActive(false);
            else if(event.key.code == fea::Keyboard::J)
                mDirectionResolver2.leftActive(false);
            else if(event.key.code == fea::Keyboard::L)
                mDirectionResolver2.rightActive(false);
        }
        else if(event.type == fea::Event::CLOSED)
        {
            mBus.send(QuitMessage());
        }
        else if(event.type == fea::Event::RESIZED)
        {
            mBus.send(ResizeMessage{{event.size.width, event.size.height}});
        }
        else if(event.type == fea::Event::MOUSEBUTTONPRESSED)
        {
            mBus.send(MouseClickMessage{{event.mouseButton.x, event.mouseButton.y}});
        }
        else if(event.type == fea::Event::MOUSEBUTTONRELEASED)
        {
            mBus.send(MouseReleaseMessage{{event.mouseButton.x, event.mouseButton.y}});
        }
        else if(event.type == fea::Event::MOUSEMOVED)
        {
            mBus.send(MouseMoveMessage{{event.mouseMove.x, event.mouseMove.y}});
        }
    }

    auto direction = mDirectionResolver1.getDirection();

    if(direction.y < 0.0f)
        actions.emplace_back(Action{0, ActionType::AIM_UP});
    else if(direction.y > 0.0f)
        actions.emplace_back(Action{0, ActionType::AIM_DOWN});

    if(direction.x < 0.0f)
        actions.emplace_back(Action{0, ActionType::WALK_LEFT});
    else if(direction.x > 0.0f)
        actions.emplace_back(Action{0, ActionType::WALK_RIGHT});

    direction = mDirectionResolver2.getDirection();

    if(direction.y < 0.0f)
        actions.emplace_back(Action{1, ActionType::AIM_UP});
    else if(direction.y > 0.0f)
        actions.emplace_back(Action{1, ActionType::AIM_DOWN});

    if(direction.x < 0.0f)
        actions.emplace_back(Action{1, ActionType::WALK_LEFT});
    else if(direction.x > 0.0f)
        actions.emplace_back(Action{1, ActionType::WALK_RIGHT});

    return actions;
}
