#include "Snek.hpp"

using namespace Snek;

void Player::ChangeSize(int size)
{
    if ((m_size + size) > 1)
    {
        m_size += size;
    }
}

void Player::SetSizeTo(int size)
{
    m_size = size;
}

size_t Player::GetSize()
{
    return m_size;
}

void Player::UpdateBody()
{
    m_snekBody.push_front(m_snekHead);
    while(m_snekBody.size() > GetSize())
    {
        m_snekBody.pop_back();
    }
}

void Player::CheckCollisionSelf()
{
        for (auto& snekBody : m_snekBody)
        {
            if ((m_snekHead.x == snekBody.x) && (m_snekHead.y == snekBody.y))
            {
                SetSizeTo(1);
                break;
            }
        }
}