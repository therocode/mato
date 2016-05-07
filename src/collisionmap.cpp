#include <thero/assert.hpp>
#include "lodepng/lodepng.hpp"
#include "collisionmap.hpp"

CollisionMap::CollisionMap(std::vector<WallType> collisionValues, glm::ivec2 size):
    mCollisionValues(std::move(collisionValues)),
    mSize(std::move(size))
{
    TH_ASSERT(mCollisionValues.size() == static_cast<size_t>(mSize.x * mSize.y), "collision values size of " << collisionValues.size() << " does not correspond to a map size of " << size.x << " " << size.y);
}

const WallType* CollisionMap::collisionValues() const
{
    return mCollisionValues.data();
}

const glm::ivec2& CollisionMap::size() const
{
    return mSize;
}

WallType CollisionMap::at(const glm::ivec2& coordinate) const
{
    TH_ASSERT(coordinate.x > 0 && coordinate.y > 0 && coordinate.x < mSize.x && coordinate.y < mSize.y, "collision map accessed out of bounds at " << coordinate.x << " " << coordinate.y);
    return mCollisionValues[coordinate.x + coordinate.y * mSize.x];
}

CollisionMap makeCollisionMap(const std::string& path)
{
    uint32_t width;
    uint32_t height;

    std::vector<unsigned char> image; //the raw pixels

    //decode
    unsigned error = lodepng::decode(image, width, height, path);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    std::vector<WallType> collisionValues(width * height, WallType::AIR);
    
    const fea::Color* colorPointer = reinterpret_cast<fea::Color*>(image.data());
    WallType* collisionPointer = collisionValues.data();

    for(size_t i = 0u; i < height * width; ++i)
    {
        *collisionPointer = colorToCollisionType(*colorPointer);
        ++colorPointer;
        ++collisionPointer;
    }

    return CollisionMap(std::move(collisionValues), {width, height});
}

WallType colorToCollisionType(const fea::Color& color)
{
    if(color == fea::Color(255, 255, 255))
    {
        return WallType::INDESTRUCTABLE_WALL;
    }
    if(color == fea::Color(0, 0, 0))
    {
        return WallType::AIR;
    }
    else
    {
        TH_ASSERT(false, "unknown color in collision map: " << +color.r << " " << +color.g << " " << +color.b << " " << +color.a);
        return WallType::AIR;
    }
}
