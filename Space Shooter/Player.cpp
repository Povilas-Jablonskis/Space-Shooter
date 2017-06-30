#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::Player()
	{

	}

	Player::Player(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), health(3), score(0)
	{

	}

	Player::~Player()
	{

	}

	bool Player::Update(std::vector<std::shared_ptr<Bullet>>* blmvector)
	{
		score += 1;
		GetAsyncKeyState(0x41) ? pressedkeys['a'] = true : pressedkeys['a'] = false;
		GetAsyncKeyState(0x44) ? pressedkeys['d'] = true : pressedkeys['d'] = false;
		GetAsyncKeyState(0x53) ? pressedkeys['s'] = true : pressedkeys['s'] = false;
		GetAsyncKeyState(0x20) ? pressedkeys[32] = true : pressedkeys[32] = false;

		if (pressedkeys[32])
			blmvector->push_back(std::make_shared<Bullet>(10, 20, glm::vec2(position[0], position[1] + height + 5.0f), glm::vec2(0.0f, 6.0f), glm::vec3(255.0f, 69.0f, 0.0f), this));

		if (pressedkeys['a'])
			position[0] -= velocity[0];
		if (pressedkeys['d'])
			position[0] += velocity[0];
		if (pressedkeys['s'])
			position[1] -= velocity[1];
		position[1] += velocity[1] / 2.0f;
		return true;
	}

	const int Player::GetHealth()
	{
		return health;
	}

	const int Player::GetScore()
	{
		return score;
	}
}