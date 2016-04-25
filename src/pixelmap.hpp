#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

class PixelMap
{
    public:
        PixelMap() = default;
        PixelMap(std::vector<uint8_t> pixels, glm::ivec2 size);
        const uint8_t* pixels() const;
        const glm::ivec2& size() const;
    private:
        std::vector<uint8_t> mPixels;
        glm::ivec2 mSize;
};
