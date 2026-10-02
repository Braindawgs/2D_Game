#pragma once

#include <SDL2/SDL.h>

enum class snakeDirection
{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct MovementVector
{
    int dx;
    int dy;
    double angle;
};

/**
 * @brief Takes keyboard input for movement
 *
 * @param evt Key event.
 * @param dir Snake directrion.
 */
void movementSelector(SDL_Event& evt, snakeDirection& dir);

/**
 * @brief Converts direction to movement step and texture angle.
 *
 * @param dir Snake direction.
 * @return MovementVector Step per axis (-1, 0, 1) and angle in degrees, zero step for NONE.
 */
MovementVector directionToMovement(snakeDirection dir);

