#include "C_PlayerLivesAndScore.hpp"

#include "C_Tag.hpp"
#include "Object.hpp"
#include "ObjectCollection.hpp"
#include "SharedContext.hpp"
#include "Sprite.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

C_PlayerLivesAndScore::C_PlayerLivesAndScore(Object* owner) : Component(owner)
{
}

void C_PlayerLivesAndScore::update(float dt)
{
	if (m_needToUpdateScoreAndHealth)
		updatePlayerLivesAndScore();
}

void C_PlayerLivesAndScore::updatePlayerLivesAndScore()
{
	m_owner->m_context->m_objects->removeObjectsWithTag(Tag::PlayerUserInterface);

	auto scoreString = std::to_string(m_playerScore);

	auto scoreTextureSize = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (auto it = scoreString.begin(); it != scoreString.end(); ++it)
	{
		const auto i = std::distance(scoreString.begin(), it);
		std::string scoreStringNumber(1, scoreString[i]);

		auto scoreNumber = std::make_shared<Object>(nullptr);
		scoreNumber->m_tag->set(Tag::PlayerUserInterface);

		const auto sprite = scoreNumber->addComponent<C_Sprite>();
		sprite->setDrawLayer(DrawLayer::UI);

		sprite->getSprite().setSpriteSheet(m_owner->m_context->m_spriteSheet);
		sprite->getSprite().setTextureRect("numeral" + scoreStringNumber + ".png");

		scoreTextureSize = sprite->getSprite().getTextureRect();

		scoreNumber->m_transform->setPosition(
			(scoreTextureSize.z * 1.5f) + sprite->getSprite().getTextureRect().z * (static_cast<float>(i) + 1.0f),
			static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (sprite->getSprite().getTextureRect().z * 3.0f));

		m_owner->m_context->m_objects->add(scoreNumber);
	}

	const auto scoreStringSize = static_cast<float>(scoreString.size());

	const auto livesIcon = std::make_shared<Object>(nullptr);
	livesIcon->m_tag->set(Tag::PlayerUserInterface);
	livesIcon->m_transform->setPosition(
		(scoreTextureSize.z * 1.5f) + scoreTextureSize.z * (scoreStringSize + 1.0f) + (scoreTextureSize.z * 2.0f),
		static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.w * 3.0f));

	const auto livesIconSprite = livesIcon->addComponent<C_Sprite>();
	livesIconSprite->setDrawLayer(DrawLayer::UI);

	livesIconSprite->getSprite().setSpriteSheet(m_owner->m_context->m_spriteSheet);
	livesIconSprite->getSprite().setTextureRect(m_livesIcon);

	m_owner->m_context->m_objects->add(livesIcon);

	const auto xIcon = std::make_shared<Object>(nullptr);
	xIcon->m_tag->set(Tag::PlayerUserInterface);
	xIcon->m_transform->setPosition(
		(scoreTextureSize.z * 1.5f) + scoreTextureSize.z * (scoreStringSize + 1.0f) + (scoreTextureSize.z * 2.0f) +
		livesIconSprite->getSprite().getTextureRect().z,
		static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.w * 3.0f));

	const auto xIconSprite = xIcon->addComponent<C_Sprite>();
	xIconSprite->setDrawLayer(DrawLayer::UI);

	xIconSprite->getSprite().setSpriteSheet(m_owner->m_context->m_spriteSheet);
	xIconSprite->getSprite().setTextureRect("numeralX.png");

	m_owner->m_context->m_objects->add(xIcon);

	auto livesString = std::to_string(m_playerLives);

	for (auto it = livesString.begin(); it != livesString.end(); ++it)
	{
		const auto i = std::distance(livesString.begin(), it);
		std::string livesStringNumber(1, livesString[i]);

		auto liveNumber = std::make_shared<Object>(nullptr);
		liveNumber->m_tag->set(Tag::PlayerUserInterface);

		const auto liveNumberSprite = liveNumber->addComponent<C_Sprite>();
		liveNumberSprite->setDrawLayer(DrawLayer::UI);

		liveNumberSprite->getSprite().setSpriteSheet(m_owner->m_context->m_spriteSheet);
		liveNumberSprite->getSprite().setTextureRect("numeral" + livesStringNumber + ".png");
		liveNumber->m_transform->setPosition(
			(scoreTextureSize.z * 1.5f) + scoreTextureSize.z * (scoreStringSize + 1.0f) + (scoreTextureSize.z * 2.0f)
			+ livesIconSprite->getSprite().getTextureRect().z + liveNumberSprite->getSprite().getTextureRect().z * (
				static_cast<float>(i) +
				1.0f), static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) - (scoreTextureSize.w * 3.0f));

		m_owner->m_context->m_objects->add(liveNumber);
	}

	m_needToUpdateScoreAndHealth = false;
}

void C_PlayerLivesAndScore::setLivesIcon(const std::string& icon)
{
	m_livesIcon = icon;
}
