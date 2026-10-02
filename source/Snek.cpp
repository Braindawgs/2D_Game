#include "Snek.hpp"

using namespace Snek;

Player::Player(int windowX, int windowY)
{
    m_sparts.snekHead.snekHead = {windowX/2, windowY/2, m_snekW, m_snekH};
    m_sparts.snekTail.snekTail = {windowX/2, windowY/2+m_snekH, m_snekW, m_snekH};

    m_sparts.snekHead.texture = {195, 3, 58, 61, snakeTexture, nullptr};
    // Normal 
    m_sparts.snekBody.texture[snekCurveTexture::NONE] = {134, 67, 51, 59, snakeTexture, nullptr};
    // Down - left
    m_sparts.snekBody.texture[snekCurveTexture::DOWN_LEFT] = {128, 128, 58, 58, snakeTexture, nullptr};
    // Up - left
    m_sparts.snekBody.texture[snekCurveTexture::UP_LEFT] = {128, 6, 58, 58, snakeTexture, nullptr};
    // Up - Right
    m_sparts.snekBody.texture[snekCurveTexture::UP_RIGHT] = {6, 6, 58, 58, snakeTexture, nullptr};
    // Down right
    m_sparts.snekBody.texture[snekCurveTexture::DOWN_RIGHT] = {6, 64, 58, 58, snakeTexture, nullptr};


    m_sparts.snekTail.texture = {199, 128, 51, 58, snakeTexture, nullptr};
    // Tail starts below head, so snake faces up and can't start by moving down.
    m_sparts.snekHead.dir = snakeDirection::UP;
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

std::vector<SDL_Rect> Player::getOccupiedCells()
{
    std::vector<SDL_Rect> cells;
    cells.push_back(m_sparts.snekHead.snekHead);
    for (auto& bodyPart : m_sparts.snekBody.snekBody)
    {
        cells.push_back(bodyPart.snekSingleBodyPart);
    }
    cells.push_back(m_sparts.snekTail.snekTail);

    return cells;
}

int Player::getSegmentSize() const
{
    return segmentSize;
}

std::deque<SnekSingleBody>& Player::getSnekBody()
{
    return m_sparts.snekBody.snekBody;
}

void Player::movementInput(SDL_Event& evt)
{
    snakeDirection newDir = m_dir;
    movementSelector(evt, newDir);

    // Compare with direction snake last moved, not last key pressed. Otherwise two quick
    // presses before next step (e.g. RIGHT then DOWN while moving UP) still reverse it.
    if (!isOppositeDirection(newDir, m_sparts.snekHead.dir))
    {
        m_dir = newDir;
    }
}

void Player::growBody()
{
    if (m_sparts.snekBody.snekBody.empty())
    {
        // Empty
        m_sparts.snekBody.snekBody.push_back({{m_sparts.snekTail.snekTail.x, m_sparts.snekTail.snekTail.y, m_snekW, m_snekH}, m_sparts.snekHead.dir});
    }
    else
    {
        m_sparts.snekBody.snekBody.push_back({m_sparts.snekBody.snekBody.back()});
    }

    m_sparts.snekTail.snekTail.x = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.x;
    m_sparts.snekTail.snekTail.y = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.y;
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
        }
        else
        {
            m_sparts.snekTail.snekTail.x = m_sparts.snekHead.snekHead.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekHead.snekHead.y;
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
    auto const movement = directionToMovement(m_dir);
    setSpeed(movement.dx, movement.dy);
    setAngle(movement.angle);

    if ((0 != m_speedX) || (0 != m_speedY))
    {
        if (!m_sparts.snekBody.snekBody.empty())
        {
            for (auto it = m_sparts.snekBody.snekBody.size() - 1 ; it > 0; it--)
            {
                m_sparts.snekBody.snekBody[it].snekSingleBodyPart.x = m_sparts.snekBody.snekBody[it - 1].snekSingleBodyPart.x;
                m_sparts.snekBody.snekBody[it].snekSingleBodyPart.y = m_sparts.snekBody.snekBody[it - 1].snekSingleBodyPart.y;
                m_sparts.snekBody.snekBody[it].dir = m_sparts.snekBody.snekBody[it - 1].dir;
            }

            // First segment to head
            m_sparts.snekBody.snekBody[0].snekSingleBodyPart.x = m_sparts.snekHead.snekHead.x;
            m_sparts.snekBody.snekBody[0].snekSingleBodyPart.y = m_sparts.snekHead.snekHead.y;
            m_sparts.snekBody.snekBody[0].dir = m_sparts.snekHead.dir;

            // Tail to last segment
            m_sparts.snekTail.snekTail.x = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekBody.snekBody.back().snekSingleBodyPart.y;
        }
        else
        {
            m_sparts.snekTail.snekTail.x = m_sparts.snekHead.snekHead.x;
            m_sparts.snekTail.snekTail.y = m_sparts.snekHead.snekHead.y;
        }
    }

    m_sparts.snekHead.snekHead.x += m_speedX * 1;
    m_sparts.snekHead.snekHead.y += m_speedY * 1;

    // Keep facing direction while standing still.
    if (snakeDirection::NONE != m_dir)
    {
        m_sparts.snekHead.dir = m_dir;
    }

}

void Player::populateTexture(Renderer& rd)
{
    m_sparts.snekHead.texture.texture = rd.loadTexture(m_sparts.snekHead.texture.sprite);
    for (auto it = 0; it < sizeof(m_sparts.snekBody.texture)/sizeof(spriteTexture); it++)
    {
        m_sparts.snekBody.texture[it].texture = rd.loadTexture(m_sparts.snekBody.texture[it].sprite);
    }
    m_sparts.snekTail.texture.texture = rd.loadTexture(m_sparts.snekTail.texture.sprite);
}

spriteTexture Player::getSnekHeadTexture() const
{
    return m_sparts.snekHead.texture;
}

spriteTexture* Player::getSnekBodyTexture()
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
                        m_sparts.snekHead.snekHead.x, m_sparts.snekHead.snekHead.y, m_sparts.snekHead.snekHead.w, m_sparts.snekHead.snekHead.h,
                        directionToMovement(m_sparts.snekHead.dir).angle);

    auto const& body = m_sparts.snekBody.snekBody;
    for (size_t it = 0; it < body.size(); it++)
    {
        // Snake left this segment in direction of the part in front of it.
        snakeDirection const outDir = (0 == it) ? m_sparts.snekHead.dir : body[it - 1].dir;
        snekCurveTexture const curve = getSnekCurve(body[it].dir, outDir);

        // Curve textures are already drawn for their turn, only straight one is rotated.
        double const angle = (snekCurveTexture::NONE == curve) ? directionToMovement(body[it].dir).angle : 0;

        rd.renderFromSpriteWithRotation(snekBodyTexture[curve].texture, snekBodyTexture[curve].spriteX, snekBodyTexture[curve].spriteY,
                                        snekBodyTexture[curve].spriteW, snekBodyTexture[curve].spriteH,
                                        body[it].snekSingleBodyPart.x, body[it].snekSingleBodyPart.y,
                                        body[it].snekSingleBodyPart.w, body[it].snekSingleBodyPart.h, angle);
    }

    rd.renderFromSpriteWithRotation(snekTailTexture.texture, snekTailTexture.spriteX, snekTailTexture.spriteY,
                    snekTailTexture.spriteW, snekTailTexture.spriteH,
                    m_sparts.snekTail.snekTail.x, m_sparts.snekTail.snekTail.y, m_sparts.snekTail.snekTail.w, m_sparts.snekTail.snekTail.h,
                    directionToMovement(getSnekTailDirection()).angle);
}

snekCurveTexture Player::getSnekCurve(snakeDirection inDir, snakeDirection outDir)
{
    snekCurveTexture curve = snekCurveTexture::NONE;

    // Each curve texture covers two turns, one clockwise and one counter clockwise.
    // Straight segments and 180 turns have no curve texture.
    switch(inDir)
    {
        case snakeDirection::UP:
        {
            if (snakeDirection::RIGHT == outDir)
            {
                curve = snekCurveTexture::UP_RIGHT;
            }
            else if (snakeDirection::LEFT == outDir)
            {
                curve = snekCurveTexture::UP_LEFT;
            }
        }
        break;
        case snakeDirection::DOWN:
        {
            if (snakeDirection::RIGHT == outDir)
            {
                curve = snekCurveTexture::DOWN_RIGHT;
            }
            else if (snakeDirection::LEFT == outDir)
            {
                curve = snekCurveTexture::DOWN_LEFT;
            }
        }
        break;
        case snakeDirection::LEFT:
        {
            if (snakeDirection::UP == outDir)
            {
                curve = snekCurveTexture::DOWN_RIGHT;
            }
            else if (snakeDirection::DOWN == outDir)
            {
                curve = snekCurveTexture::UP_RIGHT;
            }
        }
        break;
        case snakeDirection::RIGHT:
        {
            if (snakeDirection::UP == outDir)
            {
                curve = snekCurveTexture::DOWN_LEFT;
            }
            else if (snakeDirection::DOWN == outDir)
            {
                curve = snekCurveTexture::UP_LEFT;
            }
        }
        break;
        default:
        break;
    }

    return curve;
}

snakeDirection Player::getSnekTailDirection() const
{
    auto const& body = m_sparts.snekBody.snekBody;

    // Tail sits on last body segment, snake left it towards second to last segment, or head.
    return (body.size() >= 2) ? body[body.size() - 2].dir : m_sparts.snekHead.dir;
}
