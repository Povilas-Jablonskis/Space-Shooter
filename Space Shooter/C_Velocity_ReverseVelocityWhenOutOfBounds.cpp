#include "C_Velocity_ReverseVelocityWhenOutOfBounds.hpp"

#include "Object.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

C_Velocity_ReverseVelocityWhenOutOfBounds::C_Velocity_ReverseVelocityWhenOutOfBounds(Object* initialOwner) : Component(
	initialOwner)
{
}

void C_Velocity_ReverseVelocityWhenOutOfBounds::update(const float deltaTime)
{
	const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

	const auto& spriteComponentSprite = owner->getSprite()->getSprite();
	const auto transformPosition = owner->transform->getPosition();

	const auto width = spriteComponentSprite.getTextureRect().z * spriteComponentSprite.
	                                                              getScale().x;
	const auto height = spriteComponentSprite.getTextureRect().w * spriteComponentSprite.
	                                                               getScale().y;

	//Collision detection
	if (transformPosition.x + width >= windowWidth && get().x > 0.0f)
	{
		set(glm::vec2(get().x * -1.0f, get().y));
	}

	if (transformPosition.x <= 0.0f && get().x < 0.0f)
	{
		set(glm::vec2(get().x * -1.0f, get().y));
	}

	if (transformPosition.y + height >= windowHeight && get().y > 0.0f)
	{
		set(glm::vec2(get().x, get().y * -1.0f));
	}

	if (transformPosition.y <= 0.0f && get().y < 0.0f)
	{
		set(glm::vec2(get().x, get().y * -1.0f));
	}

	owner->transform->addPosition(m_velocity * deltaTime);
}

void C_Velocity_ReverseVelocityWhenOutOfBounds::set(const glm::vec2& velocity)
{
	m_velocity = velocity;
}

void C_Velocity_ReverseVelocityWhenOutOfBounds::set(const float x, const float y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

const glm::vec2& C_Velocity_ReverseVelocityWhenOutOfBounds::get() const
{
	return m_velocity;
}
