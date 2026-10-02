#pragma once

#include <SDL2/SDL.h>
#include <string>

struct spriteTexture
{
    int spriteX;
    int spriteY;
    int spriteW;
    int spriteH;
    const char* sprite;
    SDL_Texture* texture;
};


/**
 * @brief Collision detection between 2 rect obj
 * 
 * @param rectOne first object.
 * @param rectTwo second object
 * @return true if objects collided.
 */
bool checkCollision(SDL_Rect const& rectOne, SDL_Rect const& rectTwo);

/**
 * @brief Builds asset path relative to executable location, not working directory.
 *
 * @param relativePath Path relative to executable, e.g. "assets/fonts/FreeSans.ttf".
 * @return std::string Full path.
 */
std::string assetPath(std::string const& relativePath);