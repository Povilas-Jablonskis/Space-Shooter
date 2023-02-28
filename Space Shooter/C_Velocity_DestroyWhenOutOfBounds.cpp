#include "C_Velocity_DestroyWhenOutOfBounds.hpp"

#include "C_Transform.hpp"
#include "Object.hpp"
#include "Sprite.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

C_Velocity_DestroyWhenOutOfBounds::C_Velocity_DestroyWhenOutOfBounds(Object* initialOwner) : Component(initialOwner)
{
}

void C_Velocity_DestroyWhenOutOfBounds::update(const float deltaTime)
{
	const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

	const auto& spriteComponentSprite = owner->getSprite()->getSprite();
	const auto transformPosition = owner->transform->getPosition();

	const auto width = spriteComponentSprite.getTextureRect().z * spriteComponentSprite.getScale().x;
	const auto height = spriteComponentSprite.getTextureRect().w * spriteComponentSprite.getScale().y;

	//Remove if object is out of bounds
	if (transformPosition.y > windowHeight || transformPosition.y + height < 0.0f ||
		transformPosition.x > windowWidth || transformPosition.x + width < 0.0f)
	{
		owner->queueForRemoval();
	}

	owner->transform->addPosition(m_velocity * deltaTime);
}

void C_Velocity_DestroyWhenOutOfBounds::set(const glm::vec2& velocity)
{
	m_velocity = velocity;
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
