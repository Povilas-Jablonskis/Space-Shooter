#include "S_Collidable.hpp"
#include "Bitmask.hpp"
#include "Object.hpp"
#include "C_BoxCollider.hpp"
#include "C_InstanceID.hpp"

S_Collidable::S_Collidable()
{
	Bitmask playerCollisions;
	playerCollisions.setBit(static_cast<int>(CollisionLayer::Enemy));
	playerCollisions.setBit(static_cast<int>(CollisionLayer::Meteor));
	m_collisionLayers.insert(std::make_pair(CollisionLayer::Player, playerCollisions));

	Bitmask projectileCollisions;
	projectileCollisions.setBit(static_cast<int>(CollisionLayer::Enemy));
	m_collisionLayers.insert(std::make_pair(CollisionLayer::Projectile, projectileCollisions));
}

void S_Collidable::add(const std::vector<std::shared_ptr<Object>>& objects)
{
	for (const auto& o : objects)
	{
		if (auto collider = o->getComponent<C_BoxCollider>())
		{
			auto layer = collider->getLayer();

			auto itr = m_collidables.find(layer);

			if (itr != m_collidables.end())
			{
				m_collidables[layer].push_back(collider);
			}
			else
			{
				m_collidables.insert(std::make_pair(layer, std::vector{collider}));
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
	for (auto& m_collidable : m_collidables)
	{
		// If this layer collides with nothing then no need to perform any further checks.
		if (m_collisionLayers[m_collidable.first].getMask() == 0)
		{
			continue;
		}

		for (auto& collidable : m_collidable.second)
		{
			std::vector<std::shared_ptr<C_BoxCollider>> collisions;

			for (auto& it : m_collidables)
			{
				for (auto& possibleOverlap : it.second)
				{
					// Collision x-axis?
					const bool collisionX = collidable->getCollidable().x + collidable->getCollidable().z >=
						possibleOverlap->getCollidable().x &&
						possibleOverlap->getCollidable().x + possibleOverlap->getCollidable().z >= collidable->
						getCollidable().x;
					// Collision y-axis?
					const bool collisionY = collidable->getCollidable().y + collidable->getCollidable().z >=
						possibleOverlap->getCollidable().y &&
						possibleOverlap->getCollidable().y + possibleOverlap->getCollidable().z >= collidable->
						getCollidable().y;

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

				if (m_collisionLayers[collidable->getLayer()].getBit(
					static_cast<int>(collision->getLayer())))
				{
					const auto m = collidable->intersects(collision);

					if (m.colliding)
					{
						const auto collisionPair = m_objectsColliding.emplace(std::make_pair(collidable, collision));

						if (collisionPair.second)
						{
							collidable->m_owner->onCollisionEnter(*collision);
							collision->m_owner->onCollisionEnter(*collidable);
						}
					}
				}
			}
		}
	}
}

void S_Collidable::processCollidingObjects()
{
	auto itr = m_objectsColliding.begin();
	while (itr != m_objectsColliding.end())
	{
		auto& pair = *itr;

		auto& first = pair.first;
		auto& second = pair.second;

		if (first->m_owner->isQueuedForRemoval() || second->m_owner->isQueuedForRemoval())
		{
			first->m_owner->onCollisionExit(*second);
			second->m_owner->onCollisionExit(*first);

			itr = m_objectsColliding.erase(itr);
		}
		else
		{
			const auto m = first->intersects(second);

			if (!m.colliding)
			{
				first->m_owner->onCollisionExit(*second);
				second->m_owner->onCollisionExit(*first);

				itr = m_objectsColliding.erase(itr);
			}
			else
			{
				first->m_owner->onCollisionStay(*second);
				second->m_owner->onCollisionStay(*first);

				++itr;
			}
		}
	}
}
