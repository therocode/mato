#include "pixelmap.hpp"

PixelMap::PixelMap(std::vector<uint8_t> pixels, glm::ivec2 size):
    mPixels(std::move(pixels)),
    mSize(std::move(size))
{
}

const uint8_t* PixelMap::pixels() const
{
    return mPixels.data();
}

const glm::ivec2& PixelMap::size() const
{
    return mSize;
}
