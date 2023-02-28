#include "C_Collider.hpp"

C_Collider::C_Collider(Object* initialOwner) : Component(initialOwner), m_layer(CollisionLayer::DEFAULT)
{
}

CollisionLayer C_Collider::getLayer() const
{
	return m_layer;
}

void C_Collider::setLayer(const CollisionLayer layer)
{
	m_layer = layer;
}
