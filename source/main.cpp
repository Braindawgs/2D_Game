#include <SDL2/SDL.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

#define WINDOW_SIZE (1000u)
#define TEXTURE_SIZE (2000u)


static bool isOutOfBounds(SDL_Rect const& source, int ofst);

int main(int argc, char* argv[]) 
{
    SDL_Rect soruce{0, 0, WINDOW_SIZE/32, WINDOW_SIZE/32};
    SDL_Rect dest{10, 10, WINDOW_SIZE-20, WINDOW_SIZE-20};
    SDL_Event evt;

    bool running = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        throw std::runtime_error("SDL Init failed\n");
    }

    auto window = SDL_CreateWindow("Scrolling", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                WINDOW_SIZE, WINDOW_SIZE, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
                                    TEXTURE_SIZE, TEXTURE_SIZE);
    std::vector<SDL_Point> pv;
    for (int it = 0; it < 10000; it++)
    {
        pv.emplace_back(SDL_Point{static_cast<int>(rand() % TEXTURE_SIZE), static_cast<int>(rand() % TEXTURE_SIZE)});
    }
   
    while(running)
    {
        while(SDL_PollEvent(&evt))
        {
            if (SDL_QUIT == evt.type)
            {
                running = false;
            }

            else if (SDL_KEYDOWN == evt.type)
            {
                
                switch (evt.key.keysym.sym)
                {
                    case SDLK_w: 
                    {
                        if(!isOutOfBounds(soruce, -3))
                        soruce.y -=3; 
                    }
                    break;
                    case SDLK_s:
                    {
                        if (!isOutOfBounds(soruce, +3))
                        soruce.y +=3; 
                    }
                    break;

                    case SDLK_a: 
                    {
                        if (!isOutOfBounds(soruce, -3))
                        soruce.x -=3;
                    }
                    break;
                    case SDLK_d: 
                    {
                        if (!isOutOfBounds(soruce, +3))
                        soruce.x +=3;
                    }
                    break;
                    case SDLK_1:
                    {
                        if (!isOutOfBounds(soruce, soruce.w*2))
                        soruce.w *= 2;
                        soruce.h *= 2;
                    }
                    break;
                    case SDLK_2:
                    {
                        soruce.w /= 2;
                        soruce.h /= 2;
                    }
                    break;
                }
            }
        }

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        std::for_each(pv.begin(), pv.end(), [](auto& item)
        {
            item.x += rand() % 3 - 1;
            item.y += rand() % 3 - 1;
        });

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoints(renderer, pv.data(), pv.size());

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &soruce, &dest);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    // Cleanup and close
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


static bool isOutOfBounds(SDL_Rect const& source, int ofst)
{
    bool retVal = false;

    if (((source.x + ofst) > TEXTURE_SIZE) || ((source.y + ofst) > TEXTURE_SIZE) || 
        ((source.w + ofst) > TEXTURE_SIZE) || ((source.h + ofst) > TEXTURE_SIZE))
    {
        retVal = true;;
    }
    
    return retVal;
}