#include "Player.h"
#include "BulletManager.h"
#include <algorithm>
#include <functional>

namespace Engine
{
	Player::Player()
	{

	}

	Player::Player(int _width, int _height, float x, float y, float velx, float vely, float r, float g, float b) : BaseGameObject(_width, _height, x, y, velx, vely, r, g, b)
	{

	}

	Player::~Player()
	{

	}

	void Player::Update()
	{
		GetAsyncKeyState(0x41) ? pressedkeys['a'] = true : pressedkeys['a'] = false;
		GetAsyncKeyState(0x44) ? pressedkeys['d'] = true : pressedkeys['d'] = false;
		GetAsyncKeyState(0x53) ? pressedkeys['s'] = true : pressedkeys['s'] = false;
		GetAsyncKeyState(0x20) ? pressedkeys[32] = true : pressedkeys[32] = false;

		if (pressedkeys[32])
			BulletManager::AddBullet(std::shared_ptr<Bullet>(new Bullet(10, 20, position[0], position[1] + height + 5.0f, 0.0f, 3.0f, 255.0f, 69.0f, 0.0f, this)));

		if (pressedkeys['a'])
			position[0] -= 3.0f;
		if (pressedkeys['d'])
			position[0] += 3.0f;
		if (pressedkeys['s'])
			position[1] -= 1.0f;
		position[1] += 0.5f;
	}
}