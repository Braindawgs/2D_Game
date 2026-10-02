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

std::string assetPath(std::string const& relativePath)
{
    // Resolved once, executable doesn't move while running.
    static std::string const basePath = []()
    {
        std::string path;
        char* sdlPath = SDL_GetBasePath();
        if (nullptr != sdlPath)
        {
            path = sdlPath;
            SDL_free(sdlPath);
        }
        return path;
    }();

    return basePath + relativePath;
}
