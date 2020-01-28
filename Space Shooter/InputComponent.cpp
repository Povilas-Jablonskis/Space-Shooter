#include "InputComponent.hpp"
#include "Entity.hpp"
#include "InputManager.hpp"

namespace Engine
{
	void InputComponent::update(Entity* entity, const std::shared_ptr<InputManager>& inputManager, const float dt) const
	{
		entity->setDelayBetweenShootsTimer(entity->getDelayBetweenShootsTimer() + dt);
		if (entity->getDelayBetweenShootsTimer() > entity->getDelayBetweenShoots() && inputManager->getKey("Attack"))
		{
			entity->setDelayBetweenShootsTimer(0.0f);
			entity->shootingModeFunc();
		}

		if (inputManager->getKey("Move Left") || inputManager->getKey("Move Back") || inputManager->getKey("Move Right"))
		{
			auto exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			exhaust->applyAnimation(entity->getAnimationByIndex("movingExhaust"));
			exhaust->setPosition(entity->getPosition() + glm::vec2(0.0f, exhaust->getHeight() * -1.0f));
			entity->addAddon(addon("leftExhaust", exhaust));

			exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			exhaust->applyAnimation(entity->getAnimationByIndex("movingExhaust"));
			exhaust->setPosition(entity->getPosition() + glm::vec2(entity->getWidth() - exhaust->getWidth(), exhaust->getHeight() * -1.0f));
			entity->addAddon(addon("rightExhaust", exhaust));
		}
		
		if (!inputManager->getKey("Move Left") && !inputManager->getKey("Move Back") && !inputManager->getKey("Move Right"))
		{
			auto exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			exhaust->applyAnimation(entity->getAnimationByIndex("noMovingExhaust"));
			exhaust->setPosition(entity->getPosition() + glm::vec2(0.0f, exhaust->getHeight() * -1.0f));
			entity->addAddon(addon("leftExhaust", exhaust));

			exhaust = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			exhaust->applyAnimation(entity->getAnimationByIndex("noMovingExhaust"));
			exhaust->setPosition(entity->getPosition() + glm::vec2(entity->getWidth() - exhaust->getWidth(), exhaust->getHeight() * -1.0f));
			entity->addAddon(addon("rightExhaust", exhaust));
		}

		if (inputManager->getKey("Move Left"))
		{
			entity->setPosition(0, entity->getPosition().x - (entity->getVelocity().x * dt));
		}

		if (inputManager->getKey("Move Right"))
		{
			entity->setPosition(0, entity->getPosition().x + (entity->getVelocity().x * dt));
		}

		if (inputManager->getKey("Move Back"))
		{
			entity->setPosition(1, entity->getPosition().y - (entity->getVelocity().y * dt));
		}
	}
}