#include <random>
#include "Apples.hpp"
#include "Utils.hpp"

Apples::Apples(size_t count)
{
    m_count = count;
    for (auto it = 0; it < count; it++)
    {
        //Todo: change numbers
        m_apples.push_back(AppleData{SDL_Rect{rand()%100*10, rand()%100*10, 10, 10}, redApple, 0});
    }
}

void Apples::populateTexture(Renderer& rd)
{
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
    if (nullptr != entity->color.texture)
    {
        SDL_DestroyTexture(entity->color.texture);
    }
    m_apples.erase(entity);
}

size_t Apples::appleCount()
{
    return m_apples.size();
}
