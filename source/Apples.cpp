#include <iostream>
#include "Apples.hpp"
#include "Utils.hpp"

Apples::Apples(size_t count, int windowW, int windowH, int cellSize, std::vector<SDL_Rect> const& blocked)
    : m_gridW(windowW / cellSize),
      m_gridH(windowH / cellSize),
      m_cellSize(cellSize),
      m_rng(std::random_device{}())
{
    m_count = count;
    for (size_t it = 0; it < count; it++)
    {
        SDL_Rect cell;
        if (!findFreeCell(cell, blocked))
        {
            std::cerr << "No free cell for apple, placed " << m_apples.size() << " of " << count << std::endl;
            break;
        }
        m_apples.push_back(AppleData{cell, redApple, 0});
    }
}

bool Apples::isCellTaken(SDL_Rect const& cell, std::vector<SDL_Rect> const& blocked) const
{
    auto const sameCell = [&](SDL_Rect const& other) { return checkCollision(cell, other); };

    return std::any_of(blocked.begin(), blocked.end(), sameCell) ||
           std::any_of(m_apples.begin(), m_apples.end(), [&](auto const& apple) { return sameCell(apple.rect); });
}

bool Apples::findFreeCell(SDL_Rect& cell, std::vector<SDL_Rect> const& blocked)
{
    std::uniform_int_distribution<int> randX(0, m_gridW - 1);
    std::uniform_int_distribution<int> randY(0, m_gridH - 1);

    // Random tries are fast while grid is mostly empty, limit them so full grid can't loop forever.
    int const cellCount = m_gridW * m_gridH;
    for (int tries = 0; tries < cellCount; tries++)
    {
        cell = {randX(m_rng) * m_cellSize, randY(m_rng) * m_cellSize, m_cellSize, m_cellSize};
        if (!isCellTaken(cell, blocked))
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
        if (!isCellTaken(cell, blocked))
        {
            return true;
        }
    }

    return false;
}

void Apples::populateTexture(Renderer& rd)
{
    std::for_each(m_apples.begin(), m_apples.end(), [&](auto& apple)
    {
        apple.color.texture = rd.loadTexture(apple.color.sprite);
    });
}

bool Apples::checkAppleCollision(SDL_Rect const& entity)
{
    bool retVal = false;

    for (auto it = m_apples.begin(); it != m_apples.end();) 
    {
        if (checkCollision(entity, it->rect))
        {
            deleteApple(it);
            retVal = true;
            break;
        }
        else
        {
            it++;
        }
    }
    return retVal;
}

void Apples::deleteApple(std::vector<AppleData>::iterator entity)
{
    // Texture is shared and owned by Renderer.
    m_apples.erase(entity);
}

size_t Apples::appleCount()
{
    return m_apples.size();
}
