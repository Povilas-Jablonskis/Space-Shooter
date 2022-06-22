#include "C_Transform.hpp"
#include "Object.hpp"

C_Transform::C_Transform(Object* owner) : Component(owner) { }

void C_Transform::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

void C_Transform::setPosition(const glm::vec2& pos) { m_position = pos; }

void C_Transform::addPosition(float x, float y)
{
    m_position.x += x;
    m_position.y += y;
}

void C_Transform::addPosition(glm::vec2 pos) { m_position += pos; }

void C_Transform::setX(float x) { m_position.x = x; }

void C_Transform::setY(float y) { m_position.y = y; }

void C_Transform::addX(float x) { m_position.x += x; }

void C_Transform::addY(float y) { m_position.y += y; }

glm::vec2 C_Transform::getPosition() const
{
    return m_position;
}