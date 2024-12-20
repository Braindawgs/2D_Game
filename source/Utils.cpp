#include "Utils.hpp"

bool checkCollision(SDL_Rect const& rectOne, SDL_Rect const& rectTwo)
{
    bool retVal = false;

    if ((rectOne.x == rectTwo.x) && (rectOne.y == rectTwo.y))
    {
        retVal = true;
    }

    return retVal;
}
