#ifndef renderObjectH
#define renderObjectH

#include <iostream>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	class Animation;

	typedef std::pair<std::string, std::shared_ptr<Animation>> animation;

	class RenderObject
	{
		public:
			RenderObject(float, float, const glm::vec2&, const glm::vec4&);
			inline void changeColor(float color, int index) { m_color[index] = color; }
			inline void changeColor(const glm::vec4& color) { m_color = color; }
			inline void setCurrentFrame(size_t frame) { m_currentFrame = frame; }
			inline size_t getCurrentFrame() const { return m_currentFrame; }
			inline void setAnimationStatus(bool status) { m_animComplete = status; }
			inline bool getAnimationStatus() const { return m_animComplete; }
			inline void setAnimTimer(float timer) { m_animTimer = timer; }
			inline float getAnimTimer() const { return m_animTimer; }
			inline const glm::vec4& getColor() const { return m_color; }
			inline const glm::vec2& getPosition() const { return m_position; }
			inline const glm::vec3& getRotationAxis() const { return m_rotationAxis; }
			inline float getRotationAngle() const { return m_rotationAngle; }
			inline const std::shared_ptr<Animation>& getAnimation() { return m_animation; }
			virtual inline void setPosition(const glm::vec2& position) { m_position = position; }
			virtual inline void setPosition(int index, float position) { m_position[index] = position; }
			inline void setScale(float scale) { m_scale = scale; }
			inline void setRotationAngle(float rotation) { m_rotationAngle = rotation; }
			inline void setRotationAxis(const glm::vec3& rotation) { m_rotationAxis = rotation; }
			inline void setRotationAxis(int index, float rotation) { m_rotationAxis[index] = rotation; }
			inline float getWidth() const { return m_width * m_scale; };
			inline float getHeight() const { return m_height * m_scale; };
			inline void setWidth(float width) { m_width = width; }
			inline void setHeight(float height) { m_height = height; }
			inline float getOriginalWidth() const { return m_originalWidth; };
			inline float getOriginalHeight() const { return m_originalHeight; };
			inline void setOriginalWidth(float width) { m_originalWidth = width; }
			inline void setOriginalHeight(float height) { m_originalHeight = height; }
			void updateAnimation(float);
			void applyAnimation(const std::shared_ptr<Animation>&);
		private:
			inline void setAnimation(const std::shared_ptr<Animation>& t_animation) { m_animation = t_animation; }

			size_t m_currentFrame{ 0 };
			bool m_animComplete{ false };
			float m_animTimer{ 0.0f };
			std::shared_ptr<Animation> m_animation{ nullptr };
			float m_scale{ 0.5f };
			glm::vec3 m_rotationAxis{ glm::vec3(0.0, 0.0, 1.0) };
			float m_rotationAngle{ 0.0f };
			glm::vec2 m_position;
			float m_width;
			float m_height;
			glm::vec4 m_color;
			float m_originalWidth;
			float m_originalHeight;
		};
}
#endif