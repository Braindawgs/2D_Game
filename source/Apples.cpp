#include <random>
#include "Apples.hpp"

Apples::Apples(size_t count)
{
    m_count = count;
    for (auto it = 0; it < count; it++)
    {
        //Todo: change numbers
        m_apples.push_back({rand()%100*10, rand()%100*10, 10, 10});
    }
}

bool Apples::CheckCollision(SDL_Rect const& entity)
{
    bool retVal = false;
    for (auto it = m_apples.begin(); it != m_apples.end();) 
    {
        if ((entity.x == it->x) && (entity.y == it->y))
        {
            DeleteApple(*it);
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

void Apples::DeleteApple(SDL_Rect& entity)
{
    std::swap(entity, m_apples.back()); // Move the element to the end
    m_apples.pop_back();             // Remove the last element
}

size_t Apples::AppleCount()
{
    return m_apples.size();
}
