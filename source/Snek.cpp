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

void Player::updateBody()
{
    m_snekBody.push_front(m_snekHead);
    while(m_snekBody.size() > getSize())
    {
        m_snekBody.pop_back();
    }
}

void Player::checkCollisionSelf()
{
        for (auto& snekBody : m_snekBody)
        {
            if ((m_snekHead.x == snekBody.x) && (m_snekHead.y == snekBody.y))
            {
                setSizeTo(1);
                break;
            }
        }
}

void Player::movementInput(SDL_Event& evt)
{
    movementSelector(evt, m_dir);
}

void Player::updatePosition()
{
    movementExec(m_snekHead, m_dir);
}