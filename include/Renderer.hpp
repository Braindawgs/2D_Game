#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <algorithm>
#include <string>

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
        void Render(int posX, int posY, std::string const& txt, TTF_Font* font, SDL_Color const& textcolor);

        void RenderPlainRect(SDL_Rect& rect, SDL_Color const& colorscheme);

        template <typename Container>
        void RenderPlainRectArray(Container& rectV, SDL_Color const& colorscheme, bool monocolor)
        {
            if(monocolor)
            {
                SDL_SetRenderDrawColor(m_rndr, colorscheme.r, colorscheme.g, colorscheme.b, colorscheme.a);
            }

            std::for_each(rectV.begin(), rectV.end(), [&](auto& rect)
            {
                if (!monocolor)
                {
                    SDL_SetRenderDrawColor(m_rndr, colorscheme.r, colorscheme.g, colorscheme.b, colorscheme.a);
                }

                SDL_RenderFillRect(m_rndr, &rect);
            });
        }



        /**
         * @brief Presents render.
         * 
         */
        void Display();


    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_rndr = nullptr;
};