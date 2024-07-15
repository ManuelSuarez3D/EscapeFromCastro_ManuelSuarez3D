#include "Physics.h"
#include <cmath>


sf::Vector2f Physics::getOverlapEntity(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, std::string entityName, std::string state)
{
    sf::Vector2f overlap(0.f, 0.f);
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>())
        return overlap;

    auto atx = a->getComponent<CTransform>();
    auto abb = a->getComponent<CBoundingBox>();
    auto btx = b->getComponent<CTransform>();
    auto bbb = b->getComponent<CBoundingBox>();


    if (abb.has && bbb.has)
    {

        if (entityName == "default") {

            if (abb.has && bbb.has)
            {
                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(atx.pos.y - btx.pos.y);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy);
            }
        }
        if (entityName == "bullet") {

            float bottomA = atx.pos.y + abb.halfSize.y - 57.f;
            float leftA = atx.pos.x - abb.halfSize.x;
            float rightA = atx.pos.x + abb.halfSize.x;

            if (abb.has && bbb.has)
            {
                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - btx.pos.y);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy);
            }
        }
        else if (entityName == "shark") {

            float topA = 0;

            float dx = std::abs(atx.pos.x - btx.pos.x);
            float dy = std::abs(atx.pos.y - btx.pos.y);

            float bottomA = atx.pos.y + abb.halfSize.y;
            if (state == "fony")
                topA = atx.pos.y - abb.halfSize.y + 44.f;
            else if (state == "special")
                topA = atx.pos.y - abb.halfSize.y + 53.f;

            float bottomB = btx.pos.y + bbb.halfSize.y;
            float topB = btx.pos.y - bbb.halfSize.y + 18.f;

            if (dx < abb.halfSize.x + bbb.halfSize.x && dy < abb.halfSize.y + bbb.halfSize.y)
            {
                float effectiveLeftA = 0.f;
                if (state == "fony")
                    float effectiveLeftA = atx.pos.x - abb.halfSize.x + 46.f;
                else if (state == "special")
                    float effectiveLeftA = atx.pos.x - abb.halfSize.x + 56.f;

                if (effectiveLeftA < atx.pos.x - 46.f && state == "fony") {
                    effectiveLeftA = atx.pos.x - 46.f;
                }
                else if (effectiveLeftA < atx.pos.x - 56.f && state == "special")
                    effectiveLeftA = atx.pos.x - 56.f;

                float overlapLeft = std::max(0.f, std::min(atx.pos.x + abb.halfSize.x, btx.pos.x + bbb.halfSize.x) - effectiveLeftA);
                float overlapHeight = std::max(0.f, std::min(bottomA, bottomB) - std::max(topA, topB));
                overlap = sf::Vector2f(overlapLeft, overlapHeight);
            }
        }
        else if (entityName == "coral") {

            float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
            float bottomB = btx.pos.y + bbb.halfSize.y - 5.f;

            float leftB = btx.pos.x - bbb.halfSize.x + 8.f;
            float rightB = btx.pos.x + bbb.halfSize.x - 10.f;

            float leftA = atx.pos.x - abb.halfSize.x + 46.f;
            float rightA = atx.pos.x + abb.halfSize.x;

            if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
            {
                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - bottomB);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 5.f - dy);
            }
        }
        else if (entityName == "boatMilitary") {

            float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
            float bottomB = btx.pos.y + bbb.halfSize.y - 10.f;

            float leftB = btx.pos.x - bbb.halfSize.x + 30.f;
            float rightB = btx.pos.x + bbb.halfSize.x - 61.f;

            float leftA = atx.pos.x - abb.halfSize.x + 46.f;
            float rightA = atx.pos.x + abb.halfSize.x;


            if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
            {

                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - bottomB);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 10.f - dy);
            }
        }
        else if (entityName == "island") {

            float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
            float bottomB = btx.pos.y + bbb.halfSize.y - 15.f;

            float leftB = btx.pos.x - bbb.halfSize.x + 10.f;
            float rightB = btx.pos.x + bbb.halfSize.x - 19.f;


            float leftA = atx.pos.x - abb.halfSize.x + 45.f;
            float rightA = atx.pos.x + abb.halfSize.x;

            if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
            {
                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - bottomB);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 15.f - dy);
            }
        }
        else if (entityName == "coca") {

            float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
            float bottomB = btx.pos.y + bbb.halfSize.y - 5.f;

            float leftB = btx.pos.x - bbb.halfSize.x;
            float rightB = btx.pos.x + bbb.halfSize.x;

            float leftA = atx.pos.x - abb.halfSize.x + 45.f;
            float rightA = atx.pos.x + abb.halfSize.x;


            if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
            {

                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - bottomB);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 5.f - dy);
            }
        }
        else if (entityName == "squid") {

            float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
            float bottomB = btx.pos.y + bbb.halfSize.y - 21.f;

            float leftB = btx.pos.x - bbb.halfSize.x + 4.f;
            float rightB = btx.pos.x + bbb.halfSize.x - 54.f;

            float leftA = atx.pos.x - abb.halfSize.x + 46.f;
            float rightA = atx.pos.x + abb.halfSize.x;


            if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
            {

                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - bottomB);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 21.f - dy);
            }
        }
        else if (entityName == "tornado") {

            float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
            float bottomB = btx.pos.y + bbb.halfSize.y - 30.f;

            float leftB = btx.pos.x - bbb.halfSize.x + 54.f;
            float rightB = btx.pos.x + bbb.halfSize.x - 55.f;

            float leftA = atx.pos.x - abb.halfSize.x + 45.f;
            float rightA = atx.pos.x + abb.halfSize.x;

            if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
            {

                float dx = std::abs(atx.pos.x - btx.pos.x);
                float dy = std::abs(bottomA - bottomB);
                overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 30.f - dy);
            }
        }
        else if (entityName == "whirpool") {

                float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
                float bottomB = btx.pos.y + bbb.halfSize.y - 40.f;

                float leftB = btx.pos.x - bbb.halfSize.x + 15.f;
                float rightB = btx.pos.x + bbb.halfSize.x - 14.f;

                float leftA = atx.pos.x - abb.halfSize.x + 46.f;
                float rightA = atx.pos.x + abb.halfSize.x;


                if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
                {

                    float dx = std::abs(atx.pos.x - btx.pos.x);
                    float dy = std::abs(bottomA - bottomB);
                    overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 40.f - dy);
                }
            }
        else if (entityName == "boatPolice") {

                float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
                float bottomB = btx.pos.y + bbb.halfSize.y - 10.f;

                float leftB = btx.pos.x - bbb.halfSize.x + 30.f;
                float rightB = btx.pos.x + bbb.halfSize.x - 61.f;

                float leftA = atx.pos.x - abb.halfSize.x + 46.f;
                float rightA = atx.pos.x + abb.halfSize.x;


                if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
                {

                    float dx = std::abs(atx.pos.x - btx.pos.x);
                    float dy = std::abs(bottomA - bottomB);
                    overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 10.f - dy);
                }
            }
        else if (entityName == "floridaMan") {

                float bottomA = atx.pos.y + abb.halfSize.y - 6.f;
                float bottomB = btx.pos.y + bbb.halfSize.y - 6.f;

                float leftB = btx.pos.x - bbb.halfSize.x + 3.f;
                float rightB = btx.pos.x + bbb.halfSize.x - 19.f;

                float leftA = atx.pos.x - abb.halfSize.x + 45.f;
                float rightA = atx.pos.x + abb.halfSize.x;

                if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
                {

                    float dx = std::abs(atx.pos.x - btx.pos.x);
                    float dy = std::abs(bottomA - bottomB);
                    overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 6.f - dy);
                }
        }
        else if (entityName == "dune") {

                float bottomA = 0.f;
                float bottomB = 0.f;
                float leftB = 0.f;
                float rightB = 0.f;
                float leftA = 0.f;
                float rightA = 0.f;

                if (state == "fony")
                    bottomA = atx.pos.y + abb.halfSize.y - 6.f;
                else if (state == "special")
                    bottomA = atx.pos.y + abb.halfSize.y - 9.f;

                bottomB = btx.pos.y + bbb.halfSize.y - 11.f;

                leftB = btx.pos.x - bbb.halfSize.x + 2.f;
                rightB = btx.pos.x + bbb.halfSize.x - 2.f;

                if (state == "fony")
                    leftA = atx.pos.x - abb.halfSize.x + 46.f;
                else if (state == "special")
                    leftA = atx.pos.x - abb.halfSize.x + 54.f;

                rightA = atx.pos.x + abb.halfSize.x;

                if (bottomA >= bottomB && leftA <= rightB && rightA >= leftB)
                {
                    float dx = std::abs(atx.pos.x - btx.pos.x);
                    float dy = std::abs(bottomA - bottomB);
                    overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, 6.f + 11.f - dy);
                } 
        }
        return overlap;
    }
}
sf::Vector2f Physics::getOverlapMouse(sf::Vector2f a, std::shared_ptr<Entity> b)
{
    sf::Vector2f overlap(0.f, 0.f);

    auto btx = b->getComponent<CTransform>();
    auto bbb = b->getComponent<CBoundingBox>();

    if (btx.has && bbb.has)
    {
        float dx = std::abs(a.x - btx.pos.x);
        float dy = std::abs(a.y - btx.pos.y);

        float overlapX = bbb.halfSize.x - dx;
        float overlapY = bbb.halfSize.y - dy;

        overlapX = std::max(overlapX, 0.f);
        overlapY = std::max(overlapY, 0.f);

        overlap = sf::Vector2f(overlapX, overlapY);
    }

    return overlap;
}

sf::Vector2f Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    sf::Vector2f overlap(0.f, 0.f);
    if (!a->hasComponent<CBoundingBox>() or !b->hasComponent<CBoundingBox>())
        return overlap;

    auto atx = a->getComponent<CTransform>();
    auto abb = a->getComponent<CBoundingBox>();
    auto btx = b->getComponent<CTransform>();
    auto bbb = b->getComponent<CBoundingBox>();

    if (abb.has && bbb.has)
    {
        float dx = std::abs(atx.prevPos.x - btx.prevPos.x);
        float dy = std::abs(atx.prevPos.y - btx.prevPos.y);
        overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy);
    }
    return overlap;
}
