#include "C_Collider.hpp"

C_Collider::C_Collider(Object* owner) : Component(owner), m_layer(CollisionLayer::Default)
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
