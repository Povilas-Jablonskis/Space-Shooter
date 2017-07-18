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
		: BaseGameObject(_width, _height, _position, _velocity, _color), health(3), score(0)
	{

	}

	bool Player::update(InputManager* app, float _dt)
	{
		if (texture != nullptr)
			BaseGameObject::updateTexture(_dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		if (app->getKey('a'))
			position.x -= velocity.x * _dt;
		if (app->getKey('d'))
			position.x += velocity.y * _dt;
		if (app->getKey('s'))
			position.y -= velocity.y * _dt;

		position.y += (velocity.y * _dt) / 2.0f;

		if (position.x + width >= windowWidth)
			position.x = windowWidth - width;
		else if (position.x <= 0.0f)
			position.x = 0.0f;

		if (position.y + width >= windowHeigth)
			position.y = windowHeigth - height;
		else if (position.y <= 0.0f)
			position.y = 0.0f;
		return true;
	}

	void Player::reset()
	{
		setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f));
	}

	int Player::getHealth() const
	{
		return health;
	}

	void Player::setHealth(int _health)
	{
		health = _health;
	}

	int Player::getScore() const
	{
		return score;
	}

	void Player::setScore(int _score)
	{
		score = _score;
	}

	void Player::onCollision(BaseGameObject* collider)
	{
		std::cout << "player hit" << std::endl;
	}
}