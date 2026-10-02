#include <iostream>
#include "Apples.hpp"
#include "Utils.hpp"

Apples::Apples(size_t count, int windowW, int windowH, int cellSize, std::vector<SDL_Rect> const& blocked)
    : m_grid(windowW, windowH, cellSize)
{
    m_count = count;
    spawn(count, blocked);
}

size_t Apples::spawn(size_t count, std::vector<SDL_Rect> const& blocked)
{
    size_t placed = 0;
    for (; placed < count; placed++)
    {
        SDL_Rect cell;
        if (!m_grid.findFreeCell(cell, [&](SDL_Rect const& candidate) { return isCellTaken(candidate, blocked); }))
        {
            std::cerr << "No free cell for apple, placed " << placed << " of " << count << std::endl;
            break;
        }
        m_apples.push_back(AppleData{cell, redApple, 0});
    }

    return placed;
}

bool Apples::isCellTaken(SDL_Rect const& cell, std::vector<SDL_Rect> const& blocked) const
{
    auto const sameCell = [&](SDL_Rect const& other) { return checkCollision(cell, other); };

    return std::any_of(blocked.begin(), blocked.end(), sameCell) ||
           std::any_of(m_apples.begin(), m_apples.end(), [&](auto const& apple) { return sameCell(apple.rect); });
}

size_t Apples::removeInRadius(SDL_Rect const& center, int radiusCells)
{
    auto const inRadius = [&](AppleData const& apple) { return m_grid.isWithinRadius(apple.rect, center, radiusCells); };
    auto const firstRemoved = std::remove_if(m_apples.begin(), m_apples.end(), inRadius);
    size_t const removed = static_cast<size_t>(std::distance(firstRemoved, m_apples.end()));
    m_apples.erase(firstRemoved, m_apples.end());

    return removed;
}

std::vector<SDL_Rect> Apples::getOccupiedCells() const
{
    std::vector<SDL_Rect> cells;
    for (auto const& apple : m_apples)
    {
        cells.push_back(apple.rect);
    }

    return cells;
}

void Apples::populateTexture(Renderer& rd)
{
    // Templates get texture too, apples spawned later are copied from them.
    for (auto* appleTemplate : {&redApple, &greenApple, &yellowApple})
    {
        appleTemplate->texture = rd.loadTexture(appleTemplate->sprite);
    }

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
