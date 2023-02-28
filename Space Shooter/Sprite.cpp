#include "Sprite.hpp"

const glm::vec4& Sprite::getTextureRect() const
{
	return m_textureRect;
}

void Sprite::setSpriteSheet(SpriteSheet* spriteSheet)
{
	m_spriteSheet = spriteSheet;
}

const SpriteSheet& Sprite::getSpriteSheet() const
{
	return *m_spriteSheet;
}

void Sprite::changeColor(const float color, const int index)
{
	m_color[index] = color;
}

void Sprite::changeColor(const glm::vec4& color)
{
	m_color = color;
}

const glm::vec4& Sprite::getColor() const
{
	return m_color;
}

const glm::vec2& Sprite::getScale() const
{
	return m_scale;
}

void Sprite::setScale(const float scaleX, const float scaleY)
{
	m_scale = glm::vec2(scaleX, scaleY);
}

void Sprite::setRotationAngle(const float rotation)
{
	m_rotationAngle = rotation;
}

float Sprite::getRotationAngle() const
{
	return m_rotationAngle;
}

void Sprite::setRotationAxis(const glm::vec3& rotation)
{
	m_rotationAxis = rotation;
}

void Sprite::setRotationAxis(const int index, const float rotation)
{
	m_rotationAxis[index] = rotation;
}

const glm::vec3& Sprite::getRotationAxis() const
{
	return m_rotationAxis;
}

void Sprite::setTransform(const glm::mat4& transform)
{
	m_transform = transform;
}

const glm::mat4& Sprite::getTransform() const
{
	return m_transform;
}

void Sprite::setTextureRect(const std::string& name)
{
	m_textureRect = m_spriteSheet->getSprite(name);
}
