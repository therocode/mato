#include "aimgraphics.hpp"
#include "aim.hpp"
#include "pi.hpp"

AimGraphics toAimGraphics(float aim, int32_t frameAmount)
{
    bool flip = aimLeft(aim);

    aim = std::abs(aim);

    int32_t keyFrame = static_cast<int32_t>(aim / (pi / static_cast<float>(frameAmount)));

    return {keyFrame, flip};
}
