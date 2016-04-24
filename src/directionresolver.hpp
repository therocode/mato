#include <glm/glm.hpp>

class DirectionResolver
{
    public:
        void upActive(bool b);
        void downActive(bool b);
        void leftActive(bool b);
        void rightActive(bool b);
        glm::vec2 getDirection();
    private:
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
};
