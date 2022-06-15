#include "Player.hpp"
#include "InputManager.hpp"
#include "BindableActions.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

Player::Player(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color) : Entity(position, velocity, color), m_startVelocity(velocity), m_startPosition(position)
{

}

bool Player::update(const float dt, const std::shared_ptr<InputManager>& inputManager)
{
	setDelayBetweenShootsTimer(getDelayBetweenShootsTimer() + dt);
	if (getDelayBetweenShootsTimer() > getDelayBetweenShoots() && inputManager->getKey(BindableActions::ATTACK))
	{
		setDelayBetweenShootsTimer(0.0f);
		shootingModeFunc();
	}

	if (inputManager->getKey(BindableActions::MOVE_LEFT) || inputManager->getKey(BindableActions::MOVE_BACK) || inputManager->getKey(BindableActions::MOVE_RIGHT))
	{
		auto exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
		exhaust->applyAnimation(getAnimationByIndex("movingExhaust"));
		exhaust->setPosition(getPosition() + glm::vec2(0.0f, exhaust->getHeight() * -1.0f));
		addAddon("leftExhaust", exhaust);

		exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
		exhaust->applyAnimation(getAnimationByIndex("movingExhaust"));
		exhaust->setPosition(getPosition() + glm::vec2(getWidth() - exhaust->getWidth(), exhaust->getHeight() * -1.0f));
		addAddon("rightExhaust", exhaust);
	}
	else {
		auto exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
		exhaust->applyAnimation(getAnimationByIndex("noMovingExhaust"));
		exhaust->setPosition(getPosition() + glm::vec2(0.0f, exhaust->getHeight() * -1.0f));
		addAddon("leftExhaust", exhaust);

		exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
		exhaust->applyAnimation(getAnimationByIndex("noMovingExhaust"));
		exhaust->setPosition(getPosition() + glm::vec2(getWidth() - exhaust->getWidth(), exhaust->getHeight() * -1.0f));
		addAddon("rightExhaust", exhaust);
	}

	if (inputManager->getKey(BindableActions::MOVE_LEFT))
	{
		setPosition(0, getPosition().x - getVelocity().x * dt);
	}

	if (inputManager->getKey(BindableActions::MOVE_RIGHT))
	{
		setPosition(0, getPosition().x + getVelocity().x * dt);
	}

	if (inputManager->getKey(BindableActions::MOVE_BACK))
	{
		setPosition(1, getPosition().y - getVelocity().y * dt);
	}

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