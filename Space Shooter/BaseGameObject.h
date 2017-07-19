#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Application.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class BaseGameObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(int, int, glm::vec2, glm::vec2, glm::vec4);
			virtual void draw();
			virtual bool update(float);
			virtual bool update(float, float);
			virtual GLboolean checkCollision(std::shared_ptr<BaseGameObject>);
			virtual GLboolean checkCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
			void applyTexture(std::shared_ptr<Texture>);
			void setVelocity(glm::vec2);
			virtual void onCollision(BaseGameObject*);
			float getPosition(int) const;
			void setPosition(glm::vec2);
			glm::vec2 getPosition() const;
			int getSize(int) const;
			float getColor(int) const;
			void setCurrentFrame(int frame);
			int getCurrentFrame() const;
			void setDelay(float);
			void setAnimationStatus(bool);
			void setLoopStatus(bool);
			void updateTexture(float);
		protected:
			bool animComplete;
			float animTimer;
			bool loop;
			float delay;
			int currentFrame;
			std::shared_ptr<Texture> texture;
			int width;
			int height;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec4 color;
	};
}
#endif