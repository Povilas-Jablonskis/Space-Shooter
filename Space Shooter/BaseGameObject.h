#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Application.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class BaseGameObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(int, int, glm::vec2, glm::vec2, glm::vec4, std::shared_ptr<Application>);
			virtual void draw();
			virtual bool update(float);
			virtual bool update(float, float);
			virtual GLboolean checkCollision(std::shared_ptr<BaseGameObject>);
			virtual GLboolean checkCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
			void applyTexture(const std::string&);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			virtual void onCollision(BaseGameObject*);
			inline float getPosition(int index) const { return position[index]; }
			inline void setPosition(glm::vec2 _position) { position = _position; }
			inline glm::vec2 getPosition() const { return position; }
			int getSize(int) const;
			inline float getColor(int index) const { return color[index]; }
			inline void setCurrentFrame(int frame) { currentFrame = frame; }
			inline int getCurrentFrame() const { return currentFrame; }
			inline void setDelay(float _delay) { delay = _delay; }
			inline void setAnimationStatus(bool _status) { animComplete = _status; }
			inline void setLoopStatus(bool _status) { loop = _status; }
			virtual void updateTexture(float);
		protected:
			std::shared_ptr<Application> application;
			bool animComplete;
			float animTimer;
			bool loop;
			float delay;
			int currentFrame;
			std::string texture;
			int width;
			int height;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec4 color;
	};
}
#endif