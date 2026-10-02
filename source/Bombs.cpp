#include <algorithm>
#include <iostream>
#include "Bombs.hpp"
#include "Utils.hpp"

Bombs::Bombs(int windowW, int windowH, int cellSize)
    : m_grid(windowW, windowH, cellSize)
{
}

void Bombs::populateTexture(Renderer& rd)
{
    m_texture = rd.loadTexture(bombTexturePath);
}

size_t Bombs::spawn(size_t count, BombType const& type, std::vector<SDL_Rect> const& blocked)
{
    size_t placed = 0;
    for (; placed < count; placed++)
    {
        SDL_Rect cell;
        if (!m_grid.findFreeCell(cell, [&](SDL_Rect const& candidate) { return isCellTaken(candidate, blocked); }))
        {
            std::cerr << "No free cell for bomb, placed " << placed << " of " << count << std::endl;
            break;
        }
        m_bombs.push_back(BombData{cell, type, type.fuseMs, Animation(cSparkFrames, cSparkFrameMs, true)});
    }

    return placed;
}

bool Bombs::isCellTaken(SDL_Rect const& cell, std::vector<SDL_Rect> const& blocked) const
{
    auto const sameCell = [&](SDL_Rect const& other) { return checkCollision(cell, other); };

    return std::any_of(blocked.begin(), blocked.end(), sameCell) ||
           std::any_of(m_bombs.begin(), m_bombs.end(), [&](auto const& bomb) { return sameCell(bomb.rect); });
}

bool Bombs::checkBombCollision(SDL_Rect const& entity)
{
    auto const hit = std::find_if(m_bombs.begin(), m_bombs.end(), [&](auto const& bomb) { return checkCollision(entity, bomb.rect); });
    if (hit == m_bombs.end())
    {
        return false;
    }

    m_bombs.erase(hit);
    return true;
}

std::vector<Blast> Bombs::update(Uint32 deltaMs)
{
    std::vector<Blast> blasts;

    // Explosions are only visual, blast effect is applied once by game when bomb explodes.
    // Advanced before new ones are added, so new explosion starts from its first frame.
    for (auto& explosion : m_explosions)
    {
        explosion.animation.update(deltaMs);
    }
    m_explosions.erase(std::remove_if(m_explosions.begin(), m_explosions.end(), [](auto const& explosion) { return explosion.animation.isFinished(); }),
                       m_explosions.end());

    for (auto& bomb : m_bombs)
    {
        bomb.spark.update(deltaMs);
        bomb.remainingMs = (deltaMs >= bomb.remainingMs) ? 0 : (bomb.remainingMs - deltaMs);

        if (0 == bomb.remainingMs)
        {
            Blast const blast = {bomb.rect, bomb.type.blastRadius};
            blasts.push_back(blast);
            m_explosions.push_back(ExplosionData{blast, Animation(cExplosionFrames, cExplosionFrameMs, false)});
        }
    }

    m_bombs.erase(std::remove_if(m_bombs.begin(), m_bombs.end(), [](auto const& bomb) { return 0 == bomb.remainingMs; }),
                  m_bombs.end());

    return blasts;
}

bool Bombs::isInBlast(Blast const& blast, SDL_Rect const& cell) const
{
    return m_grid.isWithinRadius(cell, blast.center, blast.radiusCells);
}

bool Bombs::isHotFrame(Uint32 remainingMs)
{
    if (remainingMs > cWarningMs)
    {
        return false;
    }

    Uint32 const blinkMs = (remainingMs > cFinalWarningMs) ? cWarningBlinkMs : cFinalWarningBlinkMs;
    return 0 == ((remainingMs / blinkMs) % 2);
}

void Bombs::render(Renderer& rd)
{
    for (auto const& bomb : m_bombs)
    {
        size_t const frame = bomb.spark.getFrame() + (isHotFrame(bomb.remainingMs) ? cHotFrameOffset : 0);
        rd.renderFromSprite(m_texture, static_cast<int>(frame) * cFrameSize, cTickRow * cFrameSize, cFrameSize, cFrameSize,
                            bomb.rect.x, bomb.rect.y, bomb.rect.w, bomb.rect.h);
    }

    // Explosion covers whole blast area so player sees what got hit.
    int const cellSize = m_grid.getCellSize();
    for (auto const& explosion : m_explosions)
    {
        Blast const& blast = explosion.blast;
        int const size = (2 * blast.radiusCells + 1) * cellSize;
        int const frame = static_cast<int>(explosion.animation.getFrame());
        rd.renderFromSprite(m_texture, frame * cFrameSize, cExplosionRow * cFrameSize, cFrameSize, cFrameSize,
                            blast.center.x - blast.radiusCells * cellSize, blast.center.y - blast.radiusCells * cellSize, size, size);
    }
}

size_t Bombs::bombCount() const
{
    return m_bombs.size();
}

std::vector<SDL_Rect> Bombs::getOccupiedCells() const
{
    std::vector<SDL_Rect> cells;
    for (auto const& bomb : m_bombs)
    {
        cells.push_back(bomb.rect);
    }

    return cells;
}
