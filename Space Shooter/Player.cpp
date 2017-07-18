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

	bool Player::update(InputManager* app, float _dt, std::vector<std::shared_ptr<Bullet>>* bulletList)
	{
		if (texture != nullptr)
			texture->update(_dt);

		if (app->getKey(32))
			bulletList->push_back(std::make_shared<Bullet>(10, 20, glm::vec2(position.x, position.y + height + 5.0f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f), this));

		if (app->getKey('a'))
			position.x -= velocity.x * _dt;
		if (app->getKey('d'))
			position.x += velocity.y * _dt;
		if (app->getKey('s'))
			position.y -= velocity.y * _dt;
		position.y += (velocity.y * _dt) / 2.0f;
		return true;
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