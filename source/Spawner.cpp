#include <algorithm>
#include "Spawner.hpp"

Spawner::Spawner(SpawnRule const& rule)
    : m_rule(rule)
{
}

size_t Spawner::update(Uint32 deltaMs, size_t activeCount)
{
    // Zero interval would spawn endlessly, treat it as disabled.
    if (0 == m_rule.intervalMs)
    {
        return 0;
    }

    m_elapsedMs += deltaMs;

    // Long frame can pass more than one interval, spawn for each of them.
    size_t spawnCount = 0;
    while (m_elapsedMs >= m_rule.intervalMs)
    {
        m_elapsedMs -= m_rule.intervalMs;
        spawnCount += m_rule.count;
    }

    // Cap only limits this spawn, timer keeps running so next interval fills freed space.
    if (0 != m_rule.maxActive)
    {
        size_t const freeSlots = (activeCount < m_rule.maxActive) ? (m_rule.maxActive - activeCount) : 0;
        spawnCount = std::min(spawnCount, freeSlots);
    }

    return spawnCount;
}

void Spawner::reset()
{
    m_elapsedMs = 0;
}

SpawnRule const& Spawner::getRule() const
{
    return m_rule;
}
