#include "S_Collidable.hpp"

#include <ranges>

S_Collidable::S_Collidable()
{
	Bitmask playerCollisions;
	playerCollisions.setBit(static_cast<int>(CollisionLayer::ENEMY));
	playerCollisions.setBit(static_cast<int>(CollisionLayer::METEOR));
	m_collisionLayers.insert(std::make_pair(CollisionLayer::PLAYER, playerCollisions));

	Bitmask projectileCollisions;
	projectileCollisions.setBit(static_cast<int>(CollisionLayer::ENEMY));
	m_collisionLayers.insert(std::make_pair(CollisionLayer::PROJECTILE, projectileCollisions));
}

void S_Collidable::add(const std::vector<std::shared_ptr<Object>>& objects)
{
	for (const auto& object : objects)
	{
		if (auto collider = object->getComponent<C_BoxCollider>())
		{
			auto layer = collider->getLayer();

			auto layerIterator = m_collidables.find(layer);

			if (layerIterator != m_collidables.end())
			{
				m_collidables[layer].push_back(collider);
			}
			else
			{
				m_collidables.insert(make_pair(layer, std::vector{collider}));
			}
		}
	}
}

void S_Collidable::processRemovals()
{
	for (auto& layer : m_collidables | std::views::values)
	{
		auto layerIterator = layer.begin();
		while (layerIterator < layer.end())
		{
			if ((*layerIterator)->owner->isQueuedForRemoval())
			{
				layerIterator = layer.erase(layerIterator);
			}
			else
			{
				++layerIterator;
			}
		}
	}
}

void S_Collidable::resolve()
{
	for (auto& [layer, boxColliders] : m_collidables)
	{
		// If this layer collides with nothing then no need to perform any further checks.
		if (m_collisionLayers[layer].getMask() == 0)
		{
			continue;
		}

		for (auto& collidable : boxColliders)
		{
			std::vector<std::shared_ptr<C_BoxCollider>> collisions;

			for (auto& it : m_collidables | std::views::values)
			{
				for (auto& possibleOverlap : it)
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
				if (collidable->owner->instanceID->get() == collision->owner->instanceID->get())
				{
					continue;
				}

				if (m_collisionLayers[collidable->getLayer()].getBit(
					static_cast<int>(collision->getLayer())))
				{
					const auto [colliding, other] = collidable->intersects(collision);

					if (colliding)
					{
						const auto [firstCollider, secondCollider] = m_objectsColliding.emplace(
							make_pair(collidable, collision));

						if (secondCollider)
						{
							collidable->owner->onCollisionEnter(*collision);
							collision->owner->onCollisionEnter(*collidable);
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
		const auto& [firstCollider, secondCollider] = *itr;

		if (firstCollider->owner->isQueuedForRemoval() || secondCollider->owner->isQueuedForRemoval())
		{
			firstCollider->owner->onCollisionExit(*secondCollider);
			secondCollider->owner->onCollisionExit(*firstCollider);

			itr = m_objectsColliding.erase(itr);
		}
		else
		{
			const auto [colliding, other] = firstCollider->intersects(secondCollider);

			if (! colliding)
			{
				firstCollider->owner->onCollisionExit(*secondCollider);
				secondCollider->owner->onCollisionExit(*firstCollider);

				itr = m_objectsColliding.erase(itr);
			}
			else
			{
				firstCollider->owner->onCollisionStay(*secondCollider);
				secondCollider->owner->onCollisionStay(*firstCollider);

				++itr;
			}
		}
	}
}
