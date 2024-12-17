#include <SDL2/SDL.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <deque>

#include "Movement.hpp"

// https://www.youtube.com/watch?v=3kw1-dOikMA&list=PLYmIsLVSssdIOn5J71CVBblPlXici1_2A&index=7

#define WINDOW_SIZE (1000u)

int main(int argc, char* argv[]) 
{
    SDL_Event evt;
    SDL_Rect snekHead {WINDOW_SIZE/2, WINDOW_SIZE/2, 10, 10};

    // Snake body
    std::deque<SDL_Rect> snek;
    int size = 1;

    //Apples
    std::vector<SDL_Rect> apples;

    for (auto it = 0; it < 100; it++)
    {
        apples.push_back({rand()%100*10, rand()%100*10, 10, 10});
    }

    bool running = true;

    SnakeMove::snakeDirection dir;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        throw std::runtime_error("SDL Init failed\n");
    }

    auto window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                WINDOW_SIZE, WINDOW_SIZE, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);

    while(running)
    {
        while(SDL_PollEvent(&evt))
        {
            if (SDL_QUIT == evt.type)
            {
                running = false;
            }

            SnakeMove::movementSelecter(evt, dir);
        }

        SnakeMove::movementExec(snekHead, dir);


        // Collision detection with apple
#if 0
        std::for_each(apples.begin(), apples.end(), [&](auto& apple)
        {
            if ((snekHead.x == apple.x) && (snekHead.y == apple.y))
            {
                size +=10;
                apple.x = -10;
                apple.y = -10;
            }
        });
#endif

#if 1
        for (auto it = apples.begin(); it != apples.end(); ) 
        {
            if ((snekHead.x == it->x) && (snekHead.y == it->y)) 
            {
                size += 5; // Update size
                std::swap(*it, apples.back()); // Move the element to the end
                apples.pop_back();             // Remove the last element
            } 
            else 
            {
                ++it; // Increment only if not erasing
            }
        }
#endif

        // Collision detection with snek
        std::for_each(snek.begin(), snek.end(), [&](auto& snekBody)
        {
            if ((snekHead.x == snekBody.x) && (snekHead.y == snekBody.y))
            {
                size = 1;
            }
        });

        snek.push_front(snekHead);
        while(snek.size() > size)
        {
            snek.pop_back();
        }


        // Clear 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        std::for_each(snek.begin(), snek.end(), [&](auto& snekBody)
        {
            SDL_RenderFillRect(renderer, &snekBody);
        });

        //Draw apples
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        std::for_each(apples.begin(), apples.end(), [&](auto& apple)
        {
            SDL_RenderFillRect(renderer, &apple);
        });

        SDL_RenderPresent(renderer);
        SDL_Delay(35);
    }

    // Cleanup and close
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

