#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::~Player()
	{

	}

	Player::Player(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), health(3), score(0), startVelocity(_velocity), lastHealth(3), lastScore(0)
	{

	}

	bool Player::update(std::shared_ptr<InputManager> app, float _dt)
	{
		if (texture != nullptr)
			BaseGameObject::updateTexture(_dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		if (app->getKey('a'))
			position.x -= velocity.x * _dt;
		if (app->getKey('d'))
			position.x += velocity.x * _dt;
		if (app->getKey('s'))
			position.y -= velocity.y * _dt;

		position.y += (velocity.y * _dt) / 2.0f;

		if (position.x + width >= windowWidth)
			position.x = windowWidth - width;
		else if (position.x <= 0.0f)
			position.x = 0.0f;

		if (position.y + height >= windowHeigth)
			position.y = windowHeigth - height;
		else if (position.y <= 0.0f)
			position.y = 0.0f;
		return true;
	}

	void Player::reset()
	{
		velocity = startVelocity;
		position.x = (float)glutGet(GLUT_WINDOW_X) / 2.0f;
		position.y = 0.0f;
	}

	void Player::onCollision(BaseGameObject* collider)
	{
		std::cout << "player hit" << std::endl;
	}
}