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