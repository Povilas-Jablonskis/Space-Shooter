#include "C_Velocity.hpp"
#include "C_Transform.hpp"
#include "Object.hpp"

C_Velocity::C_Velocity(Object* owner) : Component(owner) { }

void C_Velocity::update(float deltaTime)
{
    m_owner->m_transform->addPosition(m_velocity * deltaTime);
}

void C_Velocity::set(const glm::vec2& vel)
{
    m_velocity = vel;
}

void C_Velocity::set(float x, float y)
{
    m_velocity.x = x;
    m_velocity.y = y;
}

const glm::vec2& C_Velocity::get() const
{
    return m_velocity;
}