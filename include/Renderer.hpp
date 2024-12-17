#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


class Renderer
{
    public:

        /**
         * @brief Creates a window. 
         * 
         * @param windowW Window width.
         * @param windowH Window height.
         */
        Renderer(int windowW, int windowH);

        /**
         * @brief Destroy the Renderer object.
         * 
         */
        ~Renderer();

#if 0
        /**
         * @brief Creates a window. 
         * 
         * @param windowW Window width.
         * @param windowH Window height.
         */
        void Create(int windowW, int windowH);
#endif
        /**
         * @brief Sets render to black and clears render.
         * 
         */
        void Clear();

        void Render(int posX, int posY, SDL_Texture& texture);

        void Render(SDL_Texture& texture);

        /**
         * @brief Presents render.
         * 
         */
        void Display();


    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_rndr = nullptr;
};