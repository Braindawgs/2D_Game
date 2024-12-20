#pragma once

#include <SDL2/SDL.h>

/**
 * @brief Collision detection between 2 rect obj
 * 
 * @param rectOne first object.
 * @param rectTwo second object
 * @return true if objects collided.
 */
bool checkCollision(SDL_Rect const& rectOne, SDL_Rect const& rectTwo);