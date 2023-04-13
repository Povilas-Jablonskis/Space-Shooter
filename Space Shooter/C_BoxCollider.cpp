#include "C_BoxCollider.hpp"
#include "Object.hpp"

C_BoxCollider::C_BoxCollider(Object* initialOwner) : C_Collider(initialOwner)
{
}

Manifold C_BoxCollider::intersects(const std::shared_ptr<C_Collider>& other)
{
	Manifold m;
	m.colliding = false;

	if (const auto boxCollider = dynamic_pointer_cast<C_BoxCollider>(other))
	{
		auto& rectangle = getCollidable();
		auto& boxColliderRectangle = boxCollider->getCollidable();

		// Collision x-axis?
		const bool collisionX = rectangle.x + rectangle.z >= boxColliderRectangle.x &&
			boxColliderRectangle.x + boxColliderRectangle.z >= rectangle.x;
		// Collision y-axis?
		const bool collisionY = rectangle.y + rectangle.z >= boxColliderRectangle.y &&
			boxColliderRectangle.y + boxColliderRectangle.z >= rectangle.y;

		if (collisionX && collisionY)
		{
			m.colliding = true;
			m.other = &boxColliderRectangle;
		}
	}

	return m;
}

//TODO: As we're updating the rects position manually we only need to update the width and height of the rect and an offset.
void C_BoxCollider::setCollidable(const glm::vec4& AABB)
{
	m_AABB = AABB;
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

void C_BoxCollider::setOffset(const float offsetX, const float offsetY)
{
	m_offset.x = offsetX;
	m_offset.y = offsetY;
}

void C_BoxCollider::setSize(const glm::vec2& AABB)
{
	m_AABB.z = AABB.x;
	m_AABB.w = AABB.y;
}

void C_BoxCollider::setSize(const float width, const float height)
{
	m_AABB.z = width;
	m_AABB.w = height;
}

void C_BoxCollider::setPosition()
{
	const auto& pos = owner->transform->getPosition();

	m_AABB.x = pos.x - (m_AABB.z / 2) + m_offset.x;
	m_AABB.y = pos.y - (m_AABB.w / 2) + m_offset.y;
}
