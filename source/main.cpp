#include <SDL2/SDL.h>
#include <iostream>
#include <stdexcept>

#define WINDOW_SIZE (800u)

int main(int argc, char* argv[]) 
{
    SDL_Rect rect{WINDOW_SIZE/2, WINDOW_SIZE/2, 50, 50};
    SDL_Event evt;
    bool running = true;



    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        throw std::runtime_error("SDL Init failed\n");
    }

    // Create an SDL window
    SDL_Window* window = nullptr;
    SDL_Renderer* render = nullptr; 
    if (SDL_CreateWindowAndRenderer(WINDOW_SIZE, WINDOW_SIZE, 0, &window, &render))
    {
        throw std::runtime_error("Creating window/rendered failed\n");
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
                std::cout << "Mjau" << std::endl;
            }

            else if (SDL_MOUSEMOTION == evt.type)
            {
                SDL_GetMouseState(&rect.x, &rect.y);
            }
        }

        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        SDL_RenderFillRect(render, &rect);

        SDL_RenderPresent(render);

        SDL_Delay(5);
    }

    // Cleanup and close
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
