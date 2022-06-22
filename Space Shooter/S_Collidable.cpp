#include "S_Collidable.hpp"
#include "Bitmask.hpp"
#include "Object.hpp"
#include "C_BoxCollider.hpp"
#include "C_InstanceID.hpp"

S_Collidable::S_Collidable()
{
    Bitmask defaultCollisions;
    defaultCollisions.setBit((int)CollisionLayer::Default);
    m_collisionLayers.insert(std::make_pair(CollisionLayer::Default, defaultCollisions));

    m_collisionLayers.insert(std::make_pair(CollisionLayer::Tile, Bitmask(0)));

    Bitmask playerCollisions;
    playerCollisions.setBit((int)CollisionLayer::Default);
    playerCollisions.setBit((int)CollisionLayer::Tile);
    playerCollisions.setBit((int)CollisionLayer::NPC);
    m_collisionLayers.insert(std::make_pair(CollisionLayer::Player, playerCollisions));

    Bitmask projectileCollisions;
    projectileCollisions.setBit((int)CollisionLayer::Tile);
    projectileCollisions.setBit((int)CollisionLayer::NPC);
    m_collisionLayers.insert(std::make_pair(CollisionLayer::Projectile, projectileCollisions));

    Bitmask npcCollisions;
    npcCollisions.setBit((int)CollisionLayer::Tile);
    m_collisionLayers.insert(std::make_pair(CollisionLayer::NPC, npcCollisions));
}

void S_Collidable::add(std::vector<std::shared_ptr<Object>>& objects)
{
    for (auto& o : objects)
    {
        auto collider = o->getComponent<C_BoxCollider>();
        if (collider)
        {
            CollisionLayer layer = collider->getLayer();

            auto itr = m_collidables.find(layer);

            if (itr != m_collidables.end())
            {
                m_collidables[layer].push_back(collider);
            }
            else
            {
                m_collidables.insert(std::make_pair(layer, std::vector<std::shared_ptr<C_BoxCollider>>{collider}));
            }
        }
    }
}

void S_Collidable::processRemovals()
{
    for (auto& layer : m_collidables)
    {
        auto itr = layer.second.begin();
        while (itr != layer.second.end())
        {
            if ((*itr)->m_owner->isQueuedForRemoval())
            {
                itr = layer.second.erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }
}

void S_Collidable::resolve()
{
    for (auto maps = m_collidables.begin(); maps != m_collidables.end(); ++maps)
    {
        // If this layer collides with nothing then no need to perform any furter checks.
        if (m_collisionLayers[maps->first].getMask() == 0)
        {
            continue;
        }

        for (auto& collidable : maps->second)
        {
            std::vector<std::shared_ptr<C_BoxCollider>> collisions;

            for (auto it = m_collidables.begin(); it != m_collidables.end(); ++it)
            {
                for (auto& possibleOverlap : maps->second)
                {
                    // Collision x-axis?
                    const bool collisionX = collidable->getCollidable().x + collidable->getCollidable().z >= possibleOverlap->getCollidable().x &&
                        possibleOverlap->getCollidable().x + possibleOverlap->getCollidable().z >= collidable->getCollidable().x;
                    // Collision y-axis?
                    const bool collisionY = collidable->getCollidable().y + collidable->getCollidable().z >= possibleOverlap->getCollidable().y &&
                        possibleOverlap->getCollidable().y + possibleOverlap->getCollidable().z >= collidable->getCollidable().y;

                    if (collisionX && collisionY)
                    {
                        collisions.emplace_back(possibleOverlap);
                    }
                }
            }

            for (auto& collision : collisions)
            {
                // Make sure we do not resolve collisions between the the same object.
                if (collidable->m_owner->m_instanceID->get() == collision->m_owner->m_instanceID->get())
                {
                    continue;
                }

                bool layersCollide = m_collisionLayers[collidable->getLayer()].getBit(((int)collision->getLayer()));

                if (layersCollide)
                {
                    Manifold m = collidable->intersects(collision);

                    if (m.colliding)
                    {
                        auto collisionPair = m_objectsColliding.emplace(std::make_pair(collidable, collision));

                        if (collisionPair.second)
                        {
                            collidable->m_owner->onCollisionEnter(collision);
                            collision->m_owner->onCollisionEnter(collidable);
                        }
                    }
                }
            }
        }
    }
}

void S_Collidable::update()
{
    ProcessCollidingObjects();

    resolve();
}

void S_Collidable::ProcessCollidingObjects()
{
    auto itr = m_objectsColliding.begin();
    while (itr != m_objectsColliding.end())
    {
        auto& pair = *itr;

        std::shared_ptr<C_BoxCollider> first = pair.first;
        std::shared_ptr<C_BoxCollider> second = pair.second;

        if (first->m_owner->isQueuedForRemoval() || second->m_owner->isQueuedForRemoval())
        {
            first->m_owner->onCollisionExit(second);
            second->m_owner->onCollisionExit(first);

            itr = m_objectsColliding.erase(itr);

        }
        else
        {
            Manifold m = first->intersects(second);

            if (!m.colliding)
            {
                first->m_owner->onCollisionExit(second);
                second->m_owner->onCollisionExit(first);

                itr = m_objectsColliding.erase(itr);
            }
            else
            {
                first->m_owner->onCollisionStay(second);
                second->m_owner->onCollisionStay(first);

                ++itr;
            }
        }

    }
}
