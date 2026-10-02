#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <random>

/**
 * @brief Play field split into square cells, same size as snake segment.
 * Shared by everything placed on cells (apples, bombs, ...).
 *
 */
class Grid
{
    public:
        /**
         * @brief Construct a new Grid object.
         *
         * @param windowW Window width.
         * @param windowH Window height.
         * @param cellSize Cell size in pixels.
         */
        Grid(int windowW, int windowH, int cellSize);

        /**
         * @brief Finds random cell for which isTaken returns false.
         *
         * @param cell Found cell.
         * @param isTaken Tells if cell is already used.
         * @return true If free cell was found.
         */
        bool findFreeCell(SDL_Rect& cell, std::function<bool(SDL_Rect const&)> const& isTaken);

        /**
         * @brief Checks if cell is within radius of center cell, radius counted in cells.
         *
         * @param cell Cell to check.
         * @param center Center cell.
         * @param radiusCells Radius in cells, 0 is only center cell.
         * @return true If cell is within radius.
         */
        bool isWithinRadius(SDL_Rect const& cell, SDL_Rect const& center, int radiusCells) const;

        int getCellSize() const;

    private:
        int m_gridW;
        int m_gridH;
        int m_cellSize;
        std::mt19937 m_rng;
};
