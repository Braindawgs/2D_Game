#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include "Renderer.hpp"
#include "Utils.hpp"
#include "Grid.hpp"


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
         * @brief Places new apples on free grid cells.
         *
         * @param count Number of apples to add.
         * @param blocked Cells apples must not be placed on, e.g. snake.
         * @return size_t Number of apples placed, less than count if grid is full.
         */
        size_t spawn(size_t count, std::vector<SDL_Rect> const& blocked);

        /**
         * @brief Removes all apples within radius of center cell.
         * 
         * @param center Center cell.
         * @param radiusCells Radius in cells.
         * @return size_t Number of removed apples.
         */
        size_t removeInRadius(SDL_Rect const& center, int radiusCells);

        /**
         * @brief Get cells covered by apples.
         * 
         * @return std::vector<SDL_Rect> Occupied cells.
         */
        std::vector<SDL_Rect> getOccupiedCells() const;

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

        Grid m_grid;

        void deleteApple(std::vector<AppleData>::iterator entity);

        /**
         * @brief Checks if cell is taken by apple or blocked cell.
         * 
         * @param cell Cell to check.
         * @param blocked Additional blocked cells.
         * @return true If cell is taken.
         */
        bool isCellTaken(SDL_Rect const& cell, std::vector<SDL_Rect> const& blocked) const;

};