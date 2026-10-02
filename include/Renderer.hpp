#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

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

        // Owns SDL window, renderer and textures, copying would double free them.
        Renderer(Renderer const&) = delete;
        Renderer& operator=(Renderer const&) = delete;

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
        void clear();

        /**
         * @brief Loads texture, or returns already loaded one for same path.
         * Renderer owns the texture and destroys it, callers must not.
         *
         * @param path Texture path, relative to executable location.
         * @return SDL_Texture* Texture or nullptr on failure.
         */
        SDL_Texture* loadTexture(std::string const& path);

        void render(int posX, int posY, SDL_Texture& texture);
        void render(int posX, int posY, int sizeW, int sizeH, SDL_Texture& texture);
        void render(SDL_Texture& texture);
        void render(int posX, int posY, std::string const& txt, TTF_Font* font, SDL_Color const& textColor);

        /**
         * @brief Renders text horizontally centered around centerX.
         *
         * @param centerX Horizontal center of text.
         * @param posY Top of text.
         * @param txt Text.
         * @param font Font.
         * @param textColor Text color.
         */
        void renderTextCentered(int centerX, int posY, std::string const& txt, TTF_Font* font, SDL_Color const& textColor);

        void renderFromSprite(SDL_Texture* spriteSheet, int srcX, int srcY, int srcW, int srcH, 
                             int destX, int destY, int destW, int destH);

        void renderFromSpriteWithRotation(SDL_Texture* spriteSheet, 
                    int srcX, int srcY, int srcW, int srcH, 
                    int destX, int destY, int destW, int destH, 
                    double angle, 
                    SDL_Point* rotationCenter = nullptr, 
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

        void renderFitWindow(SDL_Texture& texture);
        void renderPlainRect(SDL_Rect& rect, SDL_Color const& colorscheme);

        template <typename Container>
        void renderPlainRectArray(Container& rectV, SDL_Color const& colorScheme, bool monoColor)
        {
            if(monoColor)
            {
                SDL_SetRenderDrawColor(m_rndr, colorScheme.r, colorScheme.g, colorScheme.b, colorScheme.a);
            }

            std::for_each(rectV.begin(), rectV.end(), [&](auto& rect)
            {
                if (!monoColor)
                {
                    SDL_SetRenderDrawColor(m_rndr, colorScheme.r, colorScheme.g, colorScheme.b, colorScheme.a);
                }

                SDL_RenderFillRect(m_rndr, &rect);
            });
        }

        template <typename Container>
        void renderPlainRectTextureArray(Container& rectV, SDL_Texture& texture)
        {
            std::for_each(rectV.begin(), rectV.end(), [&](auto& rect)
            {
                SDL_RenderCopy(m_rndr, &texture, nullptr, &rectV);
            });
        }


        /**
         * @brief Presents render.
         * 
         */
        void display();


    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_rndr = nullptr;
        std::unordered_map<std::string, SDL_Texture*> m_textures;
};