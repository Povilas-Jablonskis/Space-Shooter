#include "C_PlayerLivesAndScore.hpp"

#include <memory>
#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <freeglut/freeglut.h>

#include "C_Sprite.hpp"
#include "C_Tag.hpp"
#include "Object.hpp"
#include "ObjectCollection.hpp"

C_PlayerLivesAndScore::C_PlayerLivesAndScore(Object* initialOwner) : Component(initialOwner)
{
}

void C_PlayerLivesAndScore::update(float)
{
	if (! m_needToUpdateScoreAndHealth) return;

	owner->context->objects->removeObjectsWithTag(Tag::PLAYER_USER_INTERFACE);

	auto scoreString = std::to_string(m_playerScore);

	auto scoreTextureSize = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (auto scoreStringIterator = scoreString.begin(); scoreStringIterator < scoreString.end(); ++
	     scoreStringIterator)
	{
		const auto distanceFromStartToIterator = distance(scoreString.begin(), scoreStringIterator);
		std::string scoreStringNumber(1, scoreString[distanceFromStartToIterator]);

		auto scoreNumber = std::make_shared<Object>(nullptr);
		scoreNumber->tag->set(Tag::PLAYER_USER_INTERFACE);

		const auto spriteComponent = scoreNumber->addComponent<C_Sprite>();
		spriteComponent->setDrawLayer(DrawLayer::UI);

		auto& spriteComponentSprite = spriteComponent->getSprite();

		spriteComponentSprite.setSpriteSheet(owner->context->spriteSheet);
		spriteComponentSprite.setTextureRect("numeral" + scoreStringNumber + ".png");

		scoreTextureSize = spriteComponentSprite.getTextureRect();

		scoreNumber->transform->setPosition(
			(scoreTextureSize.z * 1.5f) + scoreTextureSize.z * (static_cast<float>(distanceFromStartToIterator) + 1.0f),
			static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.z * 3.0f));

		owner->context->objects->add(scoreNumber);
	}

	const auto scoreStringSize = static_cast<float>(scoreString.size());

	const auto iconPositionX = (scoreTextureSize.z * 1.5f) + scoreTextureSize.z * (scoreStringSize + 1.0f) + (
		scoreTextureSize.z * 2.0f);

	const auto livesIcon = std::make_shared<Object>(nullptr);
	livesIcon->tag->set(Tag::PLAYER_USER_INTERFACE);

	const auto livesIconComponent = livesIcon->addComponent<C_Sprite>();
	livesIconComponent->setDrawLayer(DrawLayer::UI);

	auto& livesIconSpriteComponentSprite = livesIconComponent->getSprite();

	livesIconSpriteComponentSprite.setSpriteSheet(owner->context->spriteSheet);
	livesIconSpriteComponentSprite.setTextureRect(m_livesIcon);

	livesIcon->transform->setPosition(
		iconPositionX,
		static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.w * 3.0f));

	owner->context->objects->add(livesIcon);

	const auto xIcon = std::make_shared<Object>(nullptr);
	xIcon->tag->set(Tag::PLAYER_USER_INTERFACE);

	const auto xIconComponent = xIcon->addComponent<C_Sprite>();
	xIconComponent->setDrawLayer(DrawLayer::UI);

	auto& xIconComponentSprite = xIconComponent->getSprite();

	xIconComponentSprite.setSpriteSheet(owner->context->spriteSheet);
	xIconComponentSprite.setTextureRect("numeralX.png");

	xIcon->transform->setPosition(
		iconPositionX +
		livesIconSpriteComponentSprite.getTextureRect().z,
		static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.w * 3.0f));

	owner->context->objects->add(xIcon);

	auto livesString = std::to_string(m_playerLives);

	for (auto livesStringIterator = livesString.begin(); livesStringIterator < livesString.end(); ++
	     livesStringIterator)
	{
		const auto distanceFromStartToIterator = distance(livesString.begin(), livesStringIterator);
		std::string livesStringNumber(1, livesString[distanceFromStartToIterator]);

		auto liveNumber = std::make_shared<Object>(nullptr);
		liveNumber->tag->set(Tag::PLAYER_USER_INTERFACE);

		const auto spriteComponent = liveNumber->addComponent<C_Sprite>();
		spriteComponent->setDrawLayer(DrawLayer::UI);

		auto& spriteComponentSprite = spriteComponent->getSprite();

		spriteComponentSprite.setSpriteSheet(owner->context->spriteSheet);
		spriteComponentSprite.setTextureRect("numeral" + livesStringNumber + ".png");
		liveNumber->transform->setPosition(
			iconPositionX
			+ livesIconSpriteComponentSprite.getTextureRect().z + spriteComponentSprite.getTextureRect().z * (
				static_cast<float>(distanceFromStartToIterator) +
				1.0f), static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.w * 3.0f));

		owner->context->objects->add(liveNumber);
	}

	m_needToUpdateScoreAndHealth = false;
}

void C_PlayerLivesAndScore::setLivesIcon(const std::string& icon)
{
	m_livesIcon = icon;
}
