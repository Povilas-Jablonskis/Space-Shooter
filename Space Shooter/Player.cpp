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

	bool Player::Update(std::vector<std::shared_ptr<Bullet>>* blmvector, const Application* app)
	{
		score += 1;

		if (app->GetKey(32))
			blmvector->push_back(std::make_shared<Bullet>(10, 20, glm::vec2(position[0], position[1] + height + 5.0f), glm::vec2(0.0f, 6.0f), glm::vec3(255.0f, 69.0f, 0.0f), this));

		if (app->GetKey('a'))
			position[0] -= velocity[0];
		if (app->GetKey('d'))
			position[0] += velocity[0];
		if (app->GetKey('s'))
			position[1] -= velocity[1];
		position[1] += velocity[1] / 2.0f;
		return true;
	}

	int Player::GetHealth() const
	{
		return health;
	}

	int Player::GetScore() const
	{
		return score;
	}
}