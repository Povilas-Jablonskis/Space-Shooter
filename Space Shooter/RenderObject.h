#ifndef _renderObjectH
#define _renderObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>
#include <map>

#include "Texture.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

namespace Engine
{
	class RenderObject
	{
		public:
			RenderObject(float, float, glm::vec2, glm::vec4);
			inline void changeColor(float _color, int index) { color[index] = _color; }
			inline void changeColor(glm::vec4 _color) { color = _color; }
			inline void setCurrentFrame(int frame) { currentFrame = frame; }
			inline int getCurrentFrame() const { return currentFrame; }
			inline void setAnimationStatus(bool _status) { animComplete = _status; }
			inline bool getAnimationStatus() const { return animComplete; }
			inline void setAnimTimer(float _timer) { animTimer = _timer; }
			inline float getAnimTimer() const { return animTimer; }
			inline float getColor(int index) const{ return color[index]; }
			inline float getPosition(int index) const { return position[index]; }
			inline glm::vec2 getPosition() const { return position; }
			inline float getRotationAxis(int index) const { return rotationAxis[index]; }
			inline glm::vec3 getRotationAxis() const { return rotationAxis; }
			inline float getRotationAngle() const { return rotationAngle; }
			inline std::shared_ptr<Texture> getTexture() { return texture; }
			inline void setBBOXVar(int index, float value) { bbox[index] = value; }
			virtual inline void setPosition(glm::vec2 _position) { position = _position; }
			virtual inline void setPosition(int index, float _position) { position[index] = _position; }
			inline void setRotationAngle(float _rotation) { rotationAngle = _rotation; }
			inline void setRotationAxis(glm::vec3 _rotation) { rotationAxis = _rotation; }
			inline void setRotationAxis(int index, float _rotation) { rotationAxis[index] = _rotation; }
			float getSize(int) const;
			virtual void updateTexture(float);
			void applyTexture(std::shared_ptr<Texture>);
		protected:
			int currentFrame;
			bool animComplete;
			float animTimer;
			std::shared_ptr<Texture> texture;
			glm::vec3 rotationAxis;
			float rotationAngle;
			glm::vec2 position;
			float width;
			float height;
			glm::vec4 color;
			float bbox[4];
		};
}
#endif