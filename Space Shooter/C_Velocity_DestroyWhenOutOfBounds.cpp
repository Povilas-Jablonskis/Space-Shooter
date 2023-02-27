#include "C_Velocity_DestroyWhenOutOfBounds.hpp"

#include "C_Transform.hpp"
#include "Object.hpp"
#include "Sprite.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

C_Velocity_DestroyWhenOutOfBounds::C_Velocity_DestroyWhenOutOfBounds(Object* owner) : Component(owner)
{
}

void C_Velocity_DestroyWhenOutOfBounds::update(const float deltaTime)
{
	const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

	const auto width = m_owner->getSprite()->getSprite().getTextureRect().z * m_owner->getSprite()->getSprite().
		getScale().x;
	const auto height = m_owner->getSprite()->getSprite().getTextureRect().w * m_owner->getSprite()->getSprite().
		getScale().y;

	//Remove if object is out of bounds
	if (m_owner->m_transform->getPosition().y > windowHeight || m_owner->m_transform->getPosition().y + height < 0.0f ||
		m_owner->m_transform->getPosition().x > windowWidth || m_owner->m_transform->getPosition().x + width < 0.0f)
	{
		m_owner->queueForRemoval();
	}

	m_owner->m_transform->addPosition(m_velocity * deltaTime);
}

void C_Velocity_DestroyWhenOutOfBounds::set(const glm::vec2& vel)
{
	m_velocity = vel;
}

void C_Velocity_DestroyWhenOutOfBounds::set(const float x, const float y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

const glm::vec2& C_Velocity_DestroyWhenOutOfBounds::get() const
{
	return m_velocity;
}
