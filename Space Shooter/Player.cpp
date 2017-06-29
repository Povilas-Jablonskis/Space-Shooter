#include "Player.h"
#include "BulletManager.h"
#include <algorithm>
#include <functional>
#include "Renderer.h"
#include <string>

namespace Engine
{
	Player::Player()
	{

	}

	Player::Player(int _width, int _height, float x, float y, float velx, float vely, float r, float g, float b) : BaseGameObject(_width, _height, x, y, velx, vely, r, g, b)
	{
		health = 3;
		score = 0;
		InitUI();
	}

	Player::~Player()
	{

	}

	void Player::InitUI()
	{
		auto scoreUI = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f / 100.0f, 0.0f / 100.0f, 255.0f, 255.0f, 0.0f, 0.0f);
		auto healthUI = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f / 100.0f, 0.0f / 100.0f, 255.0f, 255.0f, 0.0f, 0.0f);

		//scoreUI
		scoreUI->AddText(std::to_string(score), 18, 6.0f / 100.0f, 92.0f / 100.0f, 255.0f, 255.0f, 255.0f, 1.0f, "AGENCYR.ttf");
		UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", scoreUI));

		//healthUI
		for (int i = 0; i < health; i++)
		{
			healthUI->AddUIElement(32, 32, (80 + (i * 6)) / 100.0f, 90.0f / 100.0f, 178.0f, 34.0f, 34.0f, 1.0f);
		}
		UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", healthUI));
	}

	void Player::Update()
	{
		for (std::map<std::string, std::shared_ptr<UIElement>>::iterator itr = UI.begin(); itr != UI.end();)
		{
			itr = UI.erase(itr);
		}
		InitUI();

		GetAsyncKeyState(0x41) ? pressedkeys['a'] = true : pressedkeys['a'] = false;
		GetAsyncKeyState(0x44) ? pressedkeys['d'] = true : pressedkeys['d'] = false;
		GetAsyncKeyState(0x53) ? pressedkeys['s'] = true : pressedkeys['s'] = false;
		GetAsyncKeyState(0x20) ? pressedkeys[32] = true : pressedkeys[32] = false;

		if (pressedkeys[32])
			BulletManager::AddBullet(std::make_shared<Bullet>(10, 20, position[0], position[1] + height + 5.0f, 0.0f, 6.0f, 255.0f, 69.0f, 0.0f, this));

		if (pressedkeys['a'])
			position[0] -= 6.0f;
		if (pressedkeys['d'])
			position[0] += 6.0f;
		if (pressedkeys['s'])
			position[1] -= 2.0f;
		position[1] += 1.0f;
	}

	void Player::DrawUI()
	{
		for (auto UIElement : UI)
		{
			Renderer::Render(UIElement.second.get());
		}
	}
}