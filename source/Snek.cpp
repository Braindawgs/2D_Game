#include "Snek.hpp"

using namespace Snek;

void Player::changeSize(int size)
{
    if ((m_size + size) > 1)
    {
        m_size += size;
    }
}

void Player::setSizeTo(int size)
{
    m_size = size;
}

size_t Player::getSize()
{
    return m_size;
}

// Todo: update for tail;
void Player::checkCollisionSelf()
{
        for (auto& snekBody : m_snekBody)
        {
            if ((m_snekHead.x == snekBody.x) && (m_snekHead.y == snekBody.y))
            {
                snekSetSize(0);
                break;
            }
        }
}

SDL_Rect& Player::getSnekHead()
{
    return m_snekHead;
}

SDL_Rect& Player::getSnekTail()
{
    return m_snekTail;
}

std::deque<SDL_Rect>& Player::getSnekBody()
{
    return m_snekBody;
}

void Player::movementInput(SDL_Event& evt)
{
    movementSelector(evt, m_dir);
}

void Player::updatePosition()
{
    movementExec(m_dir);
}

void Player::growBody()
{
    if (m_snekBody.empty())
    {
        // Empty
        m_snekBody.push_back({m_snekTail.x, m_snekTail.y, m_snekW, m_snekH});
    }
    else
    {
        m_snekBody.push_back({m_snekBody.back()});
    }

    m_snekTail.x = m_snekBody.back().x;
    m_snekTail.y = m_snekBody.back().y;
}

void Player::shrinkBody()
{
    if (!m_snekBody.empty())
    {
        m_snekBody.pop_back();
        if(!m_snekBody.empty())
        {
            m_snekTail.x = m_snekBody.back().x;
            m_snekTail.y = m_snekBody.back().y;
        }
        else
        {
            m_snekTail.x = m_snekHead.x;
            m_snekTail.y = m_snekHead.y;
        }
    }
}

void Player::snekSetSize(unsigned int size)
{   
    if (m_snekBody.size() > size)
    {
        while(m_snekBody.size() > size)
        {
            shrinkBody();
        }
    }
    else if (m_snekBody.size() < size)
    {
        while (m_snekBody.size() <= size)
        {
            growBody();
        }
    }
}

void Player::snekChangeSize(int dsize)
{
    auto snekBodySize = m_snekBody.size();
    if ((static_cast<int>(snekBodySize) + dsize) > 0)
    {
        if (dsize > 0)
        {
            for (auto it = 0; it < dsize; it++)
            {
                growBody();
            }
        }
        else if (dsize < 0)
        {
            for (auto it = 0; it < dsize*(-1); it++)
            {
                shrinkBody();
            }
        }
    }
}

void Player::setSpeed(int x, int y)
{
    m_speedX = x * segmentSize;
    m_speedY = y * segmentSize;
}

void Player::updateMovement()
{
    if ((0 != m_speedX) || (0 != m_speedY))
    {
        if (!m_snekBody.empty())
        {
            for (auto it = m_snekBody.size() - 1 ; it > 0; it--)
            {
                m_snekBody[it].x = m_snekBody[it - 1].x;
                m_snekBody[it].y = m_snekBody[it - 1].y;
            }

            // First segment to head
            m_snekBody[0].x = m_snekHead.x;
            m_snekBody[0].y = m_snekHead.y;

            // Tail to last segment
            m_snekTail.x = m_snekBody.back().x;
            m_snekTail.y = m_snekBody.back().y;
        }
        else
        {
            m_snekTail.x = m_snekHead.x;
            m_snekTail.y = m_snekHead.y;
        }
    }

    m_snekHead.x += m_speedX * 1;
    m_snekHead.y += m_speedY * 1;

}

  void Player::movementExec(snakeDirection const& dir)
    {
        switch(dir)
        {
            case snakeDirection::DOWN: 
            {
                setSpeed(0, 1);
            }
            break;
            case snakeDirection::UP: 
            {
                setSpeed(0, -1);
            }
            break;
            case snakeDirection::RIGHT: 
            {
                setSpeed(1, 0);
            }
            break;
            case snakeDirection::LEFT: 
            {
                setSpeed(-1, 0);
            }
            break;
            default:
            break;
        }
    }
