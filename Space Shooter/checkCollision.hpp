#ifndef checkCollisionH
#define checkCollisionH

#include <vector>
#include <memory>

class UIElementBase;
class Text;
class InputManager;

template <typename T, typename T2>
bool checkCollision(const std::shared_ptr<T>& object, const std::shared_ptr<T2>& collider)
{
	// Collision x-axis?
	const bool collisionX = object->getPosition().x + object->getWidth() >= collider->getPosition().x &&
		collider->getPosition().x + collider->getWidth() >= object->getPosition().x;
	// Collision y-axis?
	const bool collisionY = object->getPosition().y + object->getHeight() >= collider->getPosition().y &&
		collider->getPosition().y + collider->getHeight() >= object->getPosition().y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

template <typename T, typename T2>
bool checkCollision(const std::shared_ptr<T>& object, std::vector<std::shared_ptr<T2>>* colliderList)
{
	if (object->getNeedsToBeRemoved())
	{
		return false;
	}

	for (auto& collider : *colliderList)
	{
		if (collider->getNeedsToBeRemoved())
		{
			continue;
		}

		if (checkCollision(object, collider))
		{
			collider->onCollisionFunc(object);
			return true;
		}
	}
	return false;
}

void checkCollision(UIElementBase*, const std::shared_ptr<InputManager>&);
void checkCollision(Text*, const std::shared_ptr<InputManager>&);

#endif