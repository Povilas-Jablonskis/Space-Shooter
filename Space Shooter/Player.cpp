#include "Player.hpp"
#include "InputComponent.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

namespace Engine
{
	Player::Player(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color) : Entity(position, velocity, color), m_startVelocity(velocity), m_startPosition(position)
	{

	}

	bool Player::update(const float dt, const std::shared_ptr<InputManager>& inputManager)
	{
		getInputComponent()->update(this, inputManager, dt);

		updateAnimation(dt);

		setPosition(1, getPosition().y + getVelocity().y * dt / 2.0f);

		const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
		const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

		//Collision detection
		if (getPosition().x + getWidth() >= windowWidth)
		{
			setPosition(0, 0.0f);
		}
		else if (getPosition().x <= 0.0f)
		{
			setPosition(0, windowWidth - getWidth());
		}

		if (getPosition().y + getHeight() >= windowHeight || getPosition().y <= 0.0f)
		{
			setPosition(1, 0.0f);
		}

		auto bullets = getBulletsList();
		auto addons = getAddons();

		for (auto it = bullets->begin(); it != bullets->end();)
		{
			if ((*it)->update(dt))
			{
				it = bullets->erase(it);
			}
			else
			{
				++it;
			}
		}

		for (auto it = addons->begin(); it != addons->end();)
		{
			if ((*it).second->update(dt))
			{
				it = addons->erase(it);
			}
			else
			{
				++it;
			}
		}

		return false;
	}
}