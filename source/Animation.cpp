#include "Animation.hpp"

Animation::Animation(size_t frameCount, Uint32 frameMs, bool loop)
    : m_frameCount(frameCount),
      m_frameMs(frameMs),
      m_loop(loop)
{
}

void Animation::update(Uint32 deltaMs)
{
    m_elapsedMs += deltaMs;

    // Keep looping timer small so it can't overflow in long games.
    Uint32 const durationMs = static_cast<Uint32>(m_frameCount) * m_frameMs;
    if (m_loop && (durationMs > 0))
    {
        m_elapsedMs %= durationMs;
    }
}

size_t Animation::getFrame() const
{
    if ((0 == m_frameCount) || (0 == m_frameMs))
    {
        return 0;
    }

    size_t const frame = m_elapsedMs / m_frameMs;
    if (m_loop)
    {
        return frame % m_frameCount;
    }

    return (frame < m_frameCount) ? frame : (m_frameCount - 1);
}

bool Animation::isFinished() const
{
    Uint32 const durationMs = static_cast<Uint32>(m_frameCount) * m_frameMs;
    return !m_loop && (m_elapsedMs >= durationMs);
}

void Animation::reset()
{
    m_elapsedMs = 0;
}
