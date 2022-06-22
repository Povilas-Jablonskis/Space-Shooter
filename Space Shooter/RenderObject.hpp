#ifndef renderObjectH
#define renderObjectH

#include <iostream>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Animation;

class RenderObject
{
public:
	virtual ~RenderObject() = default;
	RenderObject(const RenderObject&) = default;
	RenderObject& operator=(const RenderObject&) = default;
	RenderObject(RenderObject&&) = default;
	RenderObject& operator=(RenderObject&&) = default;

	RenderObject(float, float, const glm::vec2&, const glm::vec4&);
	void changeColor(const float color, const int index) { m_color[index] = color; }
	void changeColor(const glm::vec4& color) { m_color = color; }
	void setCurrentFrame(const size_t frame) { m_currentFrame = frame; }
	size_t getCurrentFrame() const { return m_currentFrame; }
	void setAnimationStatus(const bool status) { m_animComplete = status; }
	bool getAnimationStatus() const { return m_animComplete; }
	void setAnimTimer(const float timer) { m_animTimer = timer; }
	float getAnimTimer() const { return m_animTimer; }
	const glm::vec4& getColor() const { return m_color; }
	const glm::vec2& getPosition() const { return m_position; }
	float getRotationAngle() const { return m_rotationAngle; }
	const std::shared_ptr<Animation>& getAnimation() const { return m_animation; }
	virtual void setPosition(const glm::vec2& position) { m_position = position; }
	virtual void setPosition(const int index, const float position) { m_position[index] = position; }
	void setScale(const float scale) { m_scale = scale; }
	void setRotationAngle(const float rotation) { m_rotationAngle = rotation; }
	float getWidth() const { return m_width * m_scale; };
	float getHeight() const { return m_height * m_scale; };
	void setWidth(const float width) { m_width = width; }
	void setHeight(const float height) { m_height = height; }
	float getOriginalWidth() const { return m_originalWidth; };
	float getOriginalHeight() const { return m_originalHeight; };
	void setOriginalWidth(const float width) { m_originalWidth = width; }
	void setOriginalHeight(const float height) { m_originalHeight = height; }
	void updateAnimation(float);
	void applyAnimation(const std::shared_ptr<Animation>&);
private:
	size_t m_currentFrame{};
	bool m_animComplete{};
	float m_animTimer{};
	std::shared_ptr<Animation> m_animation{};
	float m_scale{ 0.5f };
	float m_rotationAngle{};
	glm::vec2 m_position{};
	float m_width{};
	float m_height{};
	glm::vec4 m_color{};
	float m_originalWidth{};
	float m_originalHeight{};
};
#endif