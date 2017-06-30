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

	Player::Player(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color) 
		: BaseGameObject(_width, _height, _position, _velocity, _color), health(3), score(0)
	{
		InitUI();
	}

	Player::~Player()
	{

	}

	void Player::InitUI()
	{
		auto scoreUI = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));
		auto healthUI = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));

		//scoreUI
		scoreUI->AddText(std::to_string(score), 18, glm::vec2(6.0f / 100.0f, 92.0f / 100.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), "AGENCYR.ttf");
		UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", scoreUI));

		//healthUI
		for (int i = 0; i < health; i++)
		{
			healthUI->AddUIElement(32, 32, glm::vec2((80 + (i * 6)) / 100.0f, 90.0f / 100.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f));
		}
		UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", healthUI));
	}

	bool Player::Update()
	{
		UI.clear();
		InitUI();

		GetAsyncKeyState(0x41) ? pressedkeys['a'] = true : pressedkeys['a'] = false;
		GetAsyncKeyState(0x44) ? pressedkeys['d'] = true : pressedkeys['d'] = false;
		GetAsyncKeyState(0x53) ? pressedkeys['s'] = true : pressedkeys['s'] = false;
		GetAsyncKeyState(0x20) ? pressedkeys[32] = true : pressedkeys[32] = false;

		if (pressedkeys[32])
			BulletManager::AddBullet(10, 20, glm::vec2(position[0], position[1] + height + 5.0f), glm::vec2(0.0f, 6.0f), glm::vec3(255.0f, 69.0f, 0.0f), this);

		if (pressedkeys['a'])
			position[0] -= velocity[0];
		if (pressedkeys['d'])
			position[0] += velocity[0];
		if (pressedkeys['s'])
			position[1] -= velocity[1];
		position[1] += velocity[1] / 2.0f;
		return true;
	}

	void Player::DrawUI()
	{
		for (auto UIElement : UI)
		{
			Renderer::Render(UIElement.second.get());
		}
	}
}