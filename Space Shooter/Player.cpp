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

	Player::Player(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: Entity(_width, _height, _position, _velocity, _color), lives(0), score(0), startPosition(_position), startVelocity(_velocity)
	{
		
	}

	bool Player::update(float dt)
	{
		if (getNeedsToBeRemoved())
		{
			notify(ObserverEvent::PLAYER_DIED, std::vector<std::pair<std::string, BaseGameObject*>>());
			setNeedsToBeRemoved(false);
		}

		updateAnimation(dt);

		auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		//Collision detection
		if (getPosition(0) + getWidth() >= windowWidth)
			setPosition(0, 0.0f);
		else if (getPosition(0) <= 0.0f)
			setPosition(0, windowWidth - getWidth());

		if (getPosition(1) + getHeight() >= windowHeigth)
			setPosition(1, 0.0f);
		else if (getPosition(1) <= 0.0f)
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