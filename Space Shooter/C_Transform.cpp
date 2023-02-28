#include "C_Transform.hpp"

C_Transform::C_Transform(Object* initialOwner) : Component(initialOwner)
{
}

void C_Transform::setPosition(const float positionX, const float positionY)
{
	m_position.x = positionX;
	m_position.y = positionY;
}

void C_Transform::setPosition(const glm::vec2& position) { m_position = position; }

void C_Transform::addPosition(const float positionX, const float positionY)
{
	m_position.x += positionX;
	m_position.y += positionY;
}

void C_Transform::addPosition(const glm::vec2 position) { m_position += position; }

void C_Transform::setX(const float positionX) { m_position.x = positionX; }

void C_Transform::setY(const float positionY) { m_position.y = positionY; }

void C_Transform::addX(const float positionX) { m_position.x += positionX; }

void C_Transform::addY(const float positionY) { m_position.y += positionY; }

glm::vec2 C_Transform::getPosition() const
{
	return m_position;
}
