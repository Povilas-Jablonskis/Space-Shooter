#include "C_Velocity_ReverseVelocityWhenOutOfBounds.hpp"
#include "C_Transform.hpp"
#include "Object.hpp"
#include "Sprite.hpp"

C_Velocity_ReverseVelocityWhenOutOfBounds::C_Velocity_ReverseVelocityWhenOutOfBounds(Object* owner) : Component(owner) {}

void C_Velocity_ReverseVelocityWhenOutOfBounds::update(float deltaTime)
{
	const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

	auto width = m_owner->getSprite()->getSprite().getTextureRect().z * m_owner->getSprite()->getSprite().getScale().x;
	auto height = m_owner->getSprite()->getSprite().getTextureRect().w * m_owner->getSprite()->getSprite().getScale().y;

    //Collision detection
    if (m_owner->m_transform->getPosition().x + width >= windowWidth && get().x > 0.0f)
    {
        set(glm::vec2(get().x * -1.0f, get().y));
    }

    if (m_owner->m_transform->getPosition().x <= 0.0f && get().x < 0.0f)
    {
        set(glm::vec2(get().x * -1.0f, get().y));
    }

    if (m_owner->m_transform->getPosition().y + height >= windowHeight && get().y > 0.0f)
    {
        set(glm::vec2(get().x, get().y * -1.0f));
    }

    if (m_owner->m_transform->getPosition().y <= 0.0f && get().y < 0.0f)
    {
        set(glm::vec2(get().x, get().y * -1.0f));
    }

    m_owner->m_transform->addPosition(m_velocity * deltaTime);
}

void C_Velocity_ReverseVelocityWhenOutOfBounds::set(const glm::vec2& vel)
{
    m_velocity = vel;
}

void C_Velocity_ReverseVelocityWhenOutOfBounds::set(float x, float y)
{
    m_velocity.x = x;
    m_velocity.y = y;
}

const glm::vec2& C_Velocity_ReverseVelocityWhenOutOfBounds::get() const
{
    return m_velocity;
}