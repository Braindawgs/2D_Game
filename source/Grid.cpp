#include "Grid.hpp"

Grid::Grid(int windowW, int windowH, int cellSize)
    : m_gridW(windowW / cellSize),
      m_gridH(windowH / cellSize),
      m_cellSize(cellSize),
      m_rng(std::random_device{}())
{
}

bool Grid::findFreeCell(SDL_Rect& cell, std::function<bool(SDL_Rect const&)> const& isTaken)
{
    std::uniform_int_distribution<int> randX(0, m_gridW - 1);
    std::uniform_int_distribution<int> randY(0, m_gridH - 1);

    // Random tries are fast while grid is mostly empty, limit them so full grid can't loop forever.
    int const cellCount = m_gridW * m_gridH;
    for (int tries = 0; tries < cellCount; tries++)
    {
        cell = {randX(m_rng) * m_cellSize, randY(m_rng) * m_cellSize, m_cellSize, m_cellSize};
        if (!isTaken(cell))
        {
            return true;
        }
    }

    // Grid almost full, random tries can miss last free cells. Scan all cells from random start.
    int const start = std::uniform_int_distribution<int>(0, cellCount - 1)(m_rng);
    for (int offset = 0; offset < cellCount; offset++)
    {
        int const index = (start + offset) % cellCount;
        cell = {(index % m_gridW) * m_cellSize, (index / m_gridW) * m_cellSize, m_cellSize, m_cellSize};
        if (!isTaken(cell))
        {
            return true;
        }
    }

    return false;
}

bool Grid::isWithinRadius(SDL_Rect const& cell, SDL_Rect const& center, int radiusCells) const
{
    // Round blast, distance between cells in cell units.
    int const dx = (cell.x - center.x) / m_cellSize;
    int const dy = (cell.y - center.y) / m_cellSize;

    return (dx * dx + dy * dy) <= (radiusCells * radiusCells);
}

int Grid::getCellSize() const
{
    return m_cellSize;
}
