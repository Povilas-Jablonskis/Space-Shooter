#ifndef Sprite_hpp
#define Sprite_hpp

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "SpriteSheet.hpp"

class Sprite
{
public:
	void changeColor(const float color, const int index);
	void changeColor(const glm::vec4& color);
	[[nodiscard]] const glm::vec4& getColor() const;
	void setSpriteSheet(SpriteSheet* spriteSheet);
	[[nodiscard]] const SpriteSheet& getSpriteSheet() const;
	[[nodiscard]] const glm::vec2& getScale() const;
	void setScale(const float scaleX, const float scaleY);
	void setRotationAngle(const float rotation);
	[[nodiscard]] float getRotationAngle() const;
	void setRotationAxis(const glm::vec3& rotation);
	void setRotationAxis(const int index, const float rotation);
	[[nodiscard]] const glm::vec3& getRotationAxis() const;
	void setTransform(const glm::mat4& transform);
	[[nodiscard]] const glm::mat4& getTransform() const;
	void setTextureRect(const std::string& name);
	[[nodiscard]] const glm::vec4& getTextureRect() const;

private:
	SpriteSheet* m_spriteSheet{};
	glm::vec2 m_scale{glm::vec2(1.0f, 1.0f)};
	glm::vec3 m_rotationAxis{glm::vec3(0.0f, 0.0f, 1.0f)};
	float m_rotationAngle{};
	glm::mat4 m_transform{1.0f};
	glm::vec4 m_textureRect{};
	glm::vec4 m_color{glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)};
};

#endif
