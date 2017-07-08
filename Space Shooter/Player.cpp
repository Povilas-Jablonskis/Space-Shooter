#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::Player()
	{

	}

	Player::Player(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), health(3), score(0)
	{

	}

	Player::~Player()
	{

	}

	bool Player::Update(InputManager* app, float _dt)
	{
		if (app->GetKey(32))
			(&(bullets))->push_back(std::make_shared<Bullet>(10, 20, glm::vec2(position.x, position.y + height + 5.0f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f), this));

		BaseGameObject::UpdateBulletList(_dt);

		if (app->GetKey('a'))
			position.x -= velocity.x * _dt;
		if (app->GetKey('d'))
			position.x += velocity.y * _dt;
		if (app->GetKey('s'))
			position.y -= velocity.y * _dt;
		position.y += (velocity.y * _dt) / 2.0f;
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

	GLboolean Player::CheckCollision(std::shared_ptr<BaseGameObject> _objecttocheck) // AABB - AABB collision
	{
		if (BaseGameObject::CheckCollision(_objecttocheck))
		{
			OnCollision(_objecttocheck);
			return true;
		}
		for (auto bullet : bullets)
		{
			if (bullet->CheckCollision(_objecttocheck))
			{
				score += 100;
				return false;
			}
		}
		return false;
	}

	GLboolean Player::CheckCollision(std::vector<std::shared_ptr<BaseGameObject>>* _objectstocheck) // AABB - AABB collision
	{
		for (auto object : *_objectstocheck)
		{
			if (BaseGameObject::CheckCollision(object))
			{
				OnCollision(object);
				return true;
			}
		}
		for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it = bullets.begin(); it != bullets.end(); ++it)
		{
			for(std::vector<std::shared_ptr<BaseGameObject>>::iterator it2 = _objectstocheck->begin(); it2 != _objectstocheck->end();)
			{
				if (it->get()->CheckCollision(*it2))
				{
					_objectstocheck->erase(it2);
					bullets.erase(it);
					score += 100;
					return false;
				}
				else
					++it2;
			}
		}
		return false;
	}

	void Player::OnCollision(std::shared_ptr<BaseGameObject> collider)
	{
		std::cout << "player hit" << std::endl;
	}
}