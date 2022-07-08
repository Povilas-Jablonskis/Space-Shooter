#include "C_BoxCollider.hpp"
#include "Object.hpp"
#include "C_Transform.hpp"

C_BoxCollider::C_BoxCollider(Object* owner) : C_Collider(owner) {}

Manifold C_BoxCollider::intersects(const std::shared_ptr<C_Collider>& other)
{
    Manifold m;
    m.colliding = false;

    auto boxCollider = std::dynamic_pointer_cast<C_BoxCollider>(other);
    if (boxCollider)
    {
        auto& rect1 = getCollidable();
        auto& rect2 = boxCollider->getCollidable();

        // Collision x-axis?
        const bool collisionX = rect1.x + rect1.z >= rect2.x &&
            rect2.x + rect2.z >= rect1.x;
        // Collision y-axis?
        const bool collisionY = rect1.y + rect1.z >= rect2.y &&
            rect2.y + rect2.z >= rect1.y;

        if (collisionX && collisionY)
        {
            m.colliding = true;
            m.other = &rect2;
        }
    }

    return m;
}

//TODO: As we're updaing the rects position manually we only need to apss the width and height of the rect and an offset.
void C_BoxCollider::setCollidable(const glm::vec4& rect)
{
    m_AABB = rect;
    setPosition();
}


const glm::vec4& C_BoxCollider::getCollidable()
{
    setPosition();
    return m_AABB;
}

void C_BoxCollider::setOffset(const glm::vec2& offset)
{
    m_offset = offset; 
}

void C_BoxCollider::setOffset(float x, float y)
{
    m_offset.x = x;
    m_offset.y = y;
}

void C_BoxCollider::setSize(const glm::vec2& size)
{
    m_AABB.z = size.x;
    m_AABB.w = size.y;
}

void C_BoxCollider::setSize(float width, float height)
{
    m_AABB.z = width;
    m_AABB.w = height;
}

void C_BoxCollider::setPosition()
{
    const auto& pos = m_owner->m_transform->getPosition();

    m_AABB.x = pos.x - (m_AABB.z / 2) + m_offset.x;
    m_AABB.y = pos.y - (m_AABB.w / 2) + m_offset.y;
}