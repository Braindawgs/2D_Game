#include <SDL2/SDL.h>
#include <Movement.hpp>

    void movementSelector(SDL_Event& evt, snakeDirection& dir)
    {
        if (SDL_KEYDOWN == evt.type)
        {
            switch (evt.key.keysym.sym)
            {
                case SDLK_w: 
                {
                    dir = snakeDirection::UP;
                }
                break;
                case SDLK_s:
                {   
                    dir = snakeDirection::DOWN; 
                }
                break;

                case SDLK_a: 
                {                      
                    dir = snakeDirection::LEFT;
                }
                break;
                case SDLK_d: 
                {
                    dir = snakeDirection::RIGHT;
                }
                break;
            }
        }
    }

MovementVector directionToMovement(snakeDirection dir)
{
    MovementVector movement = {0, 0, 0};

    switch(dir)
    {
        case snakeDirection::DOWN: 
        {
            movement = {0, 1, 180};
        }
        break;
        case snakeDirection::UP: 
        {
            movement = {0, -1, 0};
        }
        break;
        case snakeDirection::RIGHT: 
        {
            movement = {1, 0, 90};
        }
        break;
        case snakeDirection::LEFT: 
        {
            movement = {-1, 0, 270};
        }
        break;
        default:
        break;
    }

    return movement;
}

bool isOppositeDirection(snakeDirection first, snakeDirection second)
{
    auto const a = directionToMovement(first);
    auto const b = directionToMovement(second);

    // Opposite when steps cancel out, NONE has no step so it is never opposite.
    bool const isMoving = (0 != a.dx) || (0 != a.dy);
    return isMoving && (a.dx == -b.dx) && (a.dy == -b.dy);
}
