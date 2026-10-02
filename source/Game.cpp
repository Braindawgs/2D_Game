#include <algorithm>
#include "Game.hpp"

static std::vector<SDL_Rect> joinCells(std::vector<SDL_Rect> cells, std::vector<SDL_Rect> const& more)
{
    cells.insert(cells.end(), more.begin(), more.end());
    return cells;
}

GameRound::GameRound(GameConfig const& config, Renderer& rd)
    : config(config),
      snek(config.windowW, config.windowH),
      apples(config.appleCount, config.windowW, config.windowH, snek.getSegmentSize(), snek.getOccupiedCells()),
      bombs(config.windowW, config.windowH, snek.getSegmentSize()),
      appleSpawner(config.appleSpawnRule),
      bombSpawner(config.bombSpawnRule)
{
    snek.populateTexture(rd);
    apples.populateTexture(rd);
    bombs.populateTexture(rd);
}

void GameRound::update(Uint32 deltaMs)
{
    snek.updateMovement();

    if (apples.checkAppleCollision(snek.getSnekHead()))
    {
        snek.snekChangeSize(static_cast<int>(config.appleGrowth));
        score++;
    }

    if (bombs.checkBombCollision(snek.getSnekHead()))
    {
        loseSegments(config.bombEatPenalty);
    }

    if (snek.checkCollisionSelf())
    {
        state = GameState::GAME_OVER;
    }

    for (auto const& blast : bombs.update(deltaMs))
    {
        explode(blast);
    }

    if (GameState::GAME_OVER == state)
    {
        return;
    }

    // Spawners only advance while playing, game over doesn't count towards next spawn.
    size_t const applesToSpawn = appleSpawner.update(deltaMs, apples.appleCount());
    if (applesToSpawn > 0)
    {
        apples.spawn(applesToSpawn, joinCells(snek.getOccupiedCells(), bombs.getOccupiedCells()));
    }

    size_t const bombsToSpawn = bombSpawner.update(deltaMs, bombs.bombCount());
    if (bombsToSpawn > 0)
    {
        bombs.spawn(bombsToSpawn, config.bombType, joinCells(snek.getOccupiedCells(), apples.getOccupiedCells()));
    }
}

void GameRound::explode(Blast const& blast)
{
    apples.removeInRadius(blast.center, blast.radiusCells);

    // Snake loses segments once per blast, no matter how many of its parts are hit.
    auto const cells = snek.getOccupiedCells();
    bool const snekHit = std::any_of(cells.begin(), cells.end(), [&](SDL_Rect const& cell) { return bombs.isInBlast(blast, cell); });
    if (snekHit)
    {
        loseSegments(config.bombBlastPenalty);
    }
}

void GameRound::loseSegments(size_t count)
{
    snek.loseSegments(count);

    // Hit that leaves no body ends game, also when snake had nothing left to lose.
    // Snake starts with no body, so only a hit can end game this way.
    if (0 == snek.getBodySize())
    {
        state = GameState::GAME_OVER;
    }
}
