#ifndef _renderObjectH
#define _renderObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>

#include "Animation.h"
#include "Renderer.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Animation>> animation;

	class RenderObject
	{
		public:
			RenderObject(float, float, glm::vec2, glm::vec4);
			inline void changeColor(float _color, int index) { color[index] = _color; }
			inline void changeColor(glm::vec4 _color) { color = _color; }
			inline void setCurrentFrame(size_t frame) { currentFrame = frame; }
			inline size_t getCurrentFrame()  { return currentFrame; }
			inline void setAnimationStatus(bool _status) { animComplete = _status; }
			inline bool getAnimationStatus()  { return animComplete; }
			inline void setAnimTimer(float _timer) { animTimer = _timer; }
			inline float getAnimTimer()  { return animTimer; }
			inline float getColor(int index) const{ return color[index]; }
			inline float getPosition(int index)  { return position[index]; }
			inline glm::vec2 getPosition()  { return position; }
			inline float getRotationAxis(int index)  { return rotationAxis[index]; }
			inline glm::vec3 getRotationAxis()  { return rotationAxis; }
			inline float getRotationAngle()  { return rotationAngle; }
			inline std::shared_ptr<Animation> getAnimation() { return theAnimation; }
			virtual inline void setPosition(glm::vec2 _position) { position = _position; }
			virtual inline void setPosition(int index, float _position) { position[index] = _position; }
			inline void setScale(float _scale) { scale = _scale; }
			inline void setRotationAngle(float _rotation) { rotationAngle = _rotation; }
			inline void setRotationAxis(glm::vec3 _rotation) { rotationAxis = _rotation; }
			inline void setRotationAxis(int index, float _rotation) { rotationAxis[index] = _rotation; }
			inline float getWidth()  { return width * scale; };
			inline float getHeight()  { return height * scale; };
			inline void setWidth(float _width) { width = _width; }
			inline void setHeight(float _height) { height = _height; }
			void updateAnimation(float);
			void applyAnimation(std::shared_ptr<Animation>);
			virtual void draw(std::shared_ptr<Renderer>);
		protected:
			size_t currentFrame;
			bool animComplete;
			float animTimer;
			std::shared_ptr<Animation> theAnimation;
			float scale;
			glm::vec3 rotationAxis;
			float rotationAngle;
			glm::vec2 position;
			float width;
			float height;
			glm::vec4 color;
			glm::vec4 bbox;
		};
}
#endif