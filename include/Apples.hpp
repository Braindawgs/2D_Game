#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <random>
#include "Renderer.hpp"
#include "Utils.hpp"


class Apples
{
    struct AppleData
    {
        SDL_Rect rect;
        spriteTexture color;
        uint8_t effect;
    };

    public:
        /**
         * @brief Places apples on free grid cells, no two apples share a cell.
         * 
         * @param count Number of apples.
         * @param windowW Window width.
         * @param windowH Window height.
         * @param cellSize Grid cell size, same as snake segment size.
         * @param blocked Cells apples must not be placed on, e.g. snake.
         */
        Apples(size_t count, int windowW, int windowH, int cellSize, std::vector<SDL_Rect> const& blocked);
        ~Apples(){};
        
        /**
         * @brief Check if entity collided with apples.
         * 
         * @param entity Collidiong object. 
         * @return true If object colided.
         */
        bool checkAppleCollision(SDL_Rect const& entity);

        /**
         * @brief Loads apple textures. 
         * 
         * @param rd Renderer.
         */
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

        int m_gridW;
        int m_gridH;
        int m_cellSize;
        std::mt19937 m_rng;

        void deleteApple(std::vector<AppleData>::iterator entity);

        /**
         * @brief Checks if cell is taken by apple or blocked cell.
         * 
         * @param cell Cell to check.
         * @param blocked Additional blocked cells.
         * @return true If cell is taken.
         */
        bool isCellTaken(SDL_Rect const& cell, std::vector<SDL_Rect> const& blocked) const;

        /**
         * @brief Finds random free cell on grid.
         * 
         * @param cell Found cell.
         * @param blocked Additional blocked cells.
         * @return true If free cell was found.
         */
        bool findFreeCell(SDL_Rect& cell, std::vector<SDL_Rect> const& blocked);
};