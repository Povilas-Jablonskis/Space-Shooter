#ifndef sprite_hpp
#define sprite_hpp

#include "Texture.hpp"

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

class Sprite
{
public:
	void changeColor(const float color, const int index) { m_color[index] = color; }
	void changeColor(const glm::vec4& color) { m_color = color; }
	const glm::vec4& getColor() const { return m_color; }
	void setTexture(const Texture& texture) { m_texture = &texture; };
	const Texture* getTexture() const { return m_texture; }
	const glm::vec2& getScale() const { return m_scale; };
	void setScale(const float x, const float y) { m_scale = glm::vec2(x, y); }
	void setRotationAngle(const float rotation) { m_rotationAngle = rotation; }
	float getRotationAngle() const { return m_rotationAngle; }
	void setRotationAxis(const glm::vec3& rotation) { m_rotationAxis = rotation; }
	void setRotationAxis(const int index, const float rotation) { m_rotationAxis[index] = rotation; }
	const glm::vec3& getRotationAxis() const { return m_rotationAxis; }
	void setTransform(const glm::mat4& transform) { m_transform = transform; };
	const glm::mat4& getTransform() const { return m_transform; };
	void setTextureRect(int x, int y, int width, int height) { m_textureRect = glm::ivec4(x, y, width, height); }
	void setTextureRect(const glm::ivec4& rect) { m_textureRect = rect; }
	const glm::ivec4& getTextureRect() const { return m_textureRect; }
private:
	const Texture* m_texture{};
	glm::vec2 m_scale{ glm::vec2(1.0f, 1.0f) };
	glm::vec3 m_rotationAxis{ glm::vec3(0.0f, 0.0f, 1.0f) };
	float m_rotationAngle{};
	glm::mat4 m_transform{ 1.0f };
	glm::ivec4 m_textureRect{};
	glm::vec4 m_color{ glm::vec4(255.0f, 255.0f, 255.0f, 1.0f) };
};

#endif