#include "C_Sprite.hpp"
#include "Object.hpp"

#include <glm/ext/matrix_transform.hpp>

C_Sprite::C_Sprite(Object* initialOwner) : Component(initialOwner)
{
}

void C_Sprite::setSortOrder(const int order)
{
	m_sortOrder = order;
}

int C_Sprite::getSortOrder() const
{
	return m_sortOrder;
}

void C_Sprite::setDrawLayer(const DrawLayer drawLayer)
{
	m_layer = drawLayer;
}

DrawLayer C_Sprite::getDrawLayer() const
{
	return m_layer;
}

//TODO: implement static sprites for level tiles etc.
void C_Sprite::update(float)
{
	auto transform = glm::mat4(1.0f);
	transform = translate(transform, glm::vec3(owner->transform->getPosition(), 0.0f));

	const auto width = m_sprite.getTextureRect().z * m_sprite.getScale().x;
	const auto height = m_sprite.getTextureRect().w * m_sprite.getScale().y;

	transform = translate(transform, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
	transform = rotate(transform, m_sprite.getRotationAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = translate(transform, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));

	transform = scale(transform, glm::vec3(width, height, 1.0f));
	m_sprite.setTransform(transform);
}

void C_Sprite::draw(const Renderer& renderer) const
{
	renderer.draw(m_sprite);
}

bool C_Sprite::continueToDraw() const
{
	return ! owner->isQueuedForRemoval();
}


Sprite& C_Sprite::getSprite()
{
	return m_sprite;
}
