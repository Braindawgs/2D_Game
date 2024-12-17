#include <SDL2/SDL.h>
#include <Movement.hpp>

namespace SnakeMove
{

    void movementSelecter(SDL_Event& evt, snakeDirection& dir)
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

    void movementExec(SDL_Rect& snekHead, snakeDirection const& dir)
    {
        switch(dir)
        {
            case snakeDirection::DOWN: 
            {
                snekHead.y += 10;
            }
            break;
            case snakeDirection::UP: 
            {
                snekHead.y -= 10;
            }
            break;
            case snakeDirection::RIGHT: 
            {
                snekHead.x += 10;
            }
            break;
            case snakeDirection::LEFT: 
            {
                snekHead.x -= 10;
            }
            break;
            default:
            break;
        }
    }
}
