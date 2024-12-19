#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include "Renderer.hpp"


class Apples
{
    struct spriteTexture
    {
        int spriteX;
        int spriteY;
        int spriteW;
        int spriteH;
        const char* sprite;
        SDL_Texture* texture;
    };

    struct AppleData
    {
        SDL_Rect rect;
        spriteTexture color;
        uint8_t effect;
    };

    public:
        Apples(size_t count);
        ~Apples(){};
        
        /**
         * @brief Check if entity collided with apples.
         * 
         * @param entity Collidiong object. 
         * @return true If object colided.
         */
        bool checkCollision(SDL_Rect const& entity);

        void populateTexture(Renderer& rd);

    	/**
    	 * @brief Apple count.
    	 * 
    	 * @return size_t count.
    	 */
        size_t appleCount();

        size_t m_count;
        std::vector<AppleData> m_apples;

    private:

        const char* appleTextureApple = "assets/textures/apples.png";
        spriteTexture redApple = {17, 124, 73, 94, appleTextureApple, nullptr};
        spriteTexture greenApple = {102, 124, 73, 94, appleTextureApple, nullptr};
        spriteTexture yellowApple = {186, 124, 73, 94, appleTextureApple, nullptr};

        void deleteApple(AppleData& entity);
};