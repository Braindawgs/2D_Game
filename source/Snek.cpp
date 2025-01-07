#include "Snek.hpp"

using namespace Snek;

Player::Player(int windowX, int windowY)
{
    m_sparts.snekHead.snekHead = {windowX/2, windowY/2, m_snekW, m_snekH};
    m_sparts.snekTail.snekTail = {windowX/2, windowY/2+m_snekH, m_snekW, m_snekH};

    m_sparts.snekHead.texture = {195, 3, 58, 61, snakeTexture, nullptr};
    // Need to load curved textures.
    m_sparts.snekBody.texture = {134, 67, 51, 59, snakeTexture, nullptr};
    m_sparts.snekTail.texture = {199, 128, 51, 58, snakeTexture, nullptr};

    m_sparts.snekHead.angle = 0;
    m_sparts.snekTail.angle = 0;

    for (auto& bodyPart : m_sparts.snekBody.snekBody) 
    {
        bodyPart.angle = 0.0;
    }

}

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
        for (auto& snekBody : m_sparts.snekBody.snekBody)
        {
            if ((m_sparts.snekHead.snekHead.x == snekBody.snekSingleBodyPart.x) && (m_sparts.snekHead.snekHead.y == snekBody.snekSingleBodyPart.y))
            {
                snekSetSize(0);
                break;
            }
        }
}

SDL_Rect& Player::getSnekHead()
{
    return m_sparts.snekHead.snekHead;
}

SDL_Rect& Player::getSnekTail()
{
    return m_sparts.snekTail.snekTail;
}

std::deque<SnekSingleBody>& Player::getSnekBody()
{
    return m_sparts.snekBody.snekBody;
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
    if (m_sparts.snekBody.snekBody.empty())
    {
        // Empty
        m_sparts.snekBody.snekBody.push_back({{m_sparts.snekTail.snekTail.x, m_sparts.snekTail.snekTail.y, m_snekW, m_snekH}, m_sparts.snekHead.angle});
    }
    else
    {
        m_sparts.snekBody.snekBody.push_back({m_sparts.snekBody.snekBody.back()});
    }

    m_sparts.snekTail.snekTail.x = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.x;
    m_sparts.snekTail.snekTail.y = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.y;
    m_sparts.snekTail.angle = m_sparts.snekBody.snekBody.back().angle;
}

void Player::shrinkBody()
{
    if (!m_sparts.snekBody.snekBody.empty())
    {
        m_sparts.snekBody.snekBody.pop_back();
        if(!m_sparts.snekBody.snekBody.empty())
        {
            m_sparts.snekTail.snekTail.x = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.y;
            m_sparts.snekTail.angle = m_sparts.snekBody.snekBody.back().angle;
        }
        else
        {
            m_sparts.snekTail.snekTail.x = m_sparts.snekHead.snekHead.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekHead.snekHead.y;
            m_sparts.snekTail.angle = m_sparts.snekHead.angle;
        }
    }
}

void Player::snekSetSize(unsigned int size)
{   
    if (m_sparts.snekBody.snekBody.size() > size)
    {
        while(m_sparts.snekBody.snekBody.size() > size)
        {
            shrinkBody();
        }
    }
    else if (m_sparts.snekBody.snekBody.size() < size)
    {
        while (m_sparts.snekBody.snekBody.size() <= size)
        {
            growBody();
        }
    }
}

void Player::snekChangeSize(int dsize)
{
    auto snekBodySize = m_sparts.snekBody.snekBody.size();
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

void Player::setAngle(double angl)
{
    m_angle = angl;
}

double Player::getAngle()
{
    return m_angle;
}

void Player::updateMovement()
{
    if ((0 != m_speedX) || (0 != m_speedY))
    {
        if (!m_sparts.snekBody.snekBody.empty())
        {
            for (auto it = m_sparts.snekBody.snekBody.size() - 1 ; it > 0; it--)
            {
                m_sparts.snekBody.snekBody[it].snekSingleBodyPart.x = m_sparts.snekBody.snekBody[it - 1].snekSingleBodyPart.x;
                m_sparts.snekBody.snekBody[it].snekSingleBodyPart.y = m_sparts.snekBody.snekBody[it - 1].snekSingleBodyPart.y;
                m_sparts.snekBody.snekBody[it].angle = m_sparts.snekBody.snekBody[it - 1].angle;
            }

            // First segment to head
            m_sparts.snekBody.snekBody[0].snekSingleBodyPart.x = m_sparts.snekHead.snekHead.x;
            m_sparts.snekBody.snekBody[0].snekSingleBodyPart.y = m_sparts.snekHead.snekHead.y;
            m_sparts.snekBody.snekBody[0].angle = m_sparts.snekHead.angle;

            // Tail to last segment
            m_sparts.snekTail.snekTail.x = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.y;
            m_sparts.snekTail.angle = m_sparts.snekBody.snekBody.back().angle; 
        }
        else
        {
            m_sparts.snekTail.snekTail.x = m_sparts.snekHead.snekHead.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekHead.snekHead.y;
            m_sparts.snekTail.angle = m_sparts.snekHead.angle;
        }
    }

    m_sparts.snekHead.snekHead.x += m_speedX * 1;
    m_sparts.snekHead.snekHead.y += m_speedY * 1;
    m_sparts.snekHead.angle = m_angle;

}

  void Player::movementExec(snakeDirection const& dir)
    {
        switch(dir)
        {
            case snakeDirection::DOWN: 
            {
                setSpeed(0, 1);
                setAngle(180);
            }
            break;
            case snakeDirection::UP: 
            {
                setSpeed(0, -1);
                setAngle(0);
            }
            break;
            case snakeDirection::RIGHT: 
            {
                setSpeed(1, 0);
                setAngle(90);
            }
            break;
            case snakeDirection::LEFT: 
            {
                setSpeed(-1, 0);
                setAngle(270);
            }
            break;
            default:
            break;
        }
    }

void Player::populateTexture(Renderer& rd)
{
    m_sparts.snekHead.texture.texture = rd.loadTexture(m_sparts.snekHead.texture.sprite);
    m_sparts.snekBody.texture.texture = rd.loadTexture(m_sparts.snekBody.texture.sprite);
    m_sparts.snekTail.texture.texture = rd.loadTexture(m_sparts.snekTail.texture.sprite);
}

spriteTexture Player::getSnekHeadTexture() const
{
    return m_sparts.snekHead.texture;
}

spriteTexture Player::getSnekBodyTexture() const
{
    return m_sparts.snekBody.texture;
}

spriteTexture Player::getSnekTailTexture() const
{
    return m_sparts.snekTail.texture;
}

void Player::renderSnake(Renderer& rd)
{
    auto snekHeadTexture = getSnekHeadTexture();
    auto snekBodyTexture = getSnekBodyTexture();
    auto snekTailTexture = getSnekTailTexture();

    rd.renderFromSpriteWithRotation(snekHeadTexture.texture, snekHeadTexture.spriteX, snekHeadTexture.spriteY, 
                        snekHeadTexture.spriteW, snekHeadTexture.spriteH,
                        m_sparts.snekHead.snekHead.x, m_sparts.snekHead.snekHead.y, m_sparts.snekHead.snekHead.w, m_sparts.snekHead.snekHead.h, m_sparts.snekHead.angle);

    if (!m_sparts.snekBody.snekBody.empty())
    {
        for(auto& snekBody : m_sparts.snekBody.snekBody)
        {
            rd.renderFromSpriteWithRotation(snekBodyTexture.texture, snekBodyTexture.spriteX, snekBodyTexture.spriteY, 
                                            snekBodyTexture.spriteW, snekBodyTexture.spriteH, 
                                            snekBody.snekSingleBodyPart.x, snekBody.snekSingleBodyPart.y,
                                            snekBody.snekSingleBodyPart.w, snekBody.snekSingleBodyPart.h, snekBody.angle);
        }
    }

    rd.renderFromSpriteWithRotation(snekTailTexture.texture, snekTailTexture.spriteX, snekTailTexture.spriteY, 
                    snekTailTexture.spriteW, snekTailTexture.spriteH,
                    m_sparts.snekTail.snekTail.x, m_sparts.snekTail.snekTail.y, m_sparts.snekTail.snekTail.w, m_sparts.snekTail.snekTail.h,  m_sparts.snekTail.angle);
}