#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::~Player()
	{
		clearBullets();
	}

	Player::Player(glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, std::shared_ptr<InputComponent> input)
		: Entity(_position, _velocity, _color, input), score(0), startPosition(_position), startVelocity(_velocity)
	{
		onCollisionFunc = [this](std::shared_ptr<BaseGameObject> collider)
		{
			
		};
	}

	bool Player::update(float dt, std::shared_ptr<InputManager> inputManager)
	{
		if (getNeedsToBeRemoved())
		{
			setLives(getLives() - 1);
			setVelocity(getStartVelocity());
			setPosition(getStartPosition());
			if (getLives() < 1)
			{
				notify(ObserverEvent::PLAYER_DIED);
			}

			setNeedsToBeRemoved(false);
		}

		getInputComponent()->update(this, inputManager, dt);

		updateAnimation(dt);

		setPosition(1, getPosition().y + ((getVelocity().y * dt) / 2.0f));

		auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		//Collision detection
		if (getPosition().x + getWidth() >= windowWidth)
			setPosition(0, 0.0f);
		else if (getPosition().x <= 0.0f)
			setPosition(0, windowWidth - getWidth());

		if (getPosition().y + getHeight() >= windowHeigth)
			setPosition(1, 0.0f);
		else if (getPosition().y <= 0.0f)
			setPosition(1, 0.0f);

		auto bullets = getBulletsList();
		auto addons = getAddons();

		for (auto it = bullets->begin(); it != bullets->end();)
		{
			if ((*it)->update(dt))
				it = bullets->erase(it);
			else
				++it;
		}

		for (auto it = addons->begin(); it != addons->end();)
		{
			if ((*it).second->update(dt))
				it = addons->erase(it);
			else
				++it;
		}

		return false;
	}
}