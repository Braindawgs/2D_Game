#pragma once

enum snakeDirection
{
    UP = 1,
    DOWN,
    LEFT, 
    RIGHT,
    NONE
};

/**
 * @brief Takes keyboard input for movement
 * 
 * @param evt Key event.
 * @param dir Snake directrion.
 */
void movementSelector(SDL_Event& evt, snakeDirection& dir);

/**
 * @brief Executed snake direction.
 * 
 * @param snekHead Snake head structure.
 * @param dir Selected direction.
 */
void movementExec(SDL_Rect& snekHead, snakeDirection const& dir);

