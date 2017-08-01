#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Texture.h"

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
			BaseGameObject(int, int, glm::vec2, glm::vec2, glm::vec4);
			void draw(GLuint);
			virtual bool update(float);
			virtual void applyTexture(std::shared_ptr<Texture>);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline glm::vec2 getVelocity() { return velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			virtual void onCollision(BaseGameObject*);
			inline float getPosition(int index) const { return position[index]; }
			inline void setPosition(glm::vec2 _position) { position = _position; }
			inline void setPosition(int index, float _position) { position[index] = _position; }
			inline glm::vec2 getPosition() const { return position; }
			int getSize(int) const;
			inline float getColor(int index) const { return color[index]; }
			inline std::vector<std::shared_ptr<BaseGameObject>>* getBulletsList() { return &bullets; }
			inline std::shared_ptr<Texture> getTexture() { return texture; }
			virtual void updateTexture(float);
			void addBullet(std::shared_ptr<BaseGameObject>, std::shared_ptr<Texture> = nullptr);
		protected:
			std::vector<std::shared_ptr<BaseGameObject>> bullets;
			std::shared_ptr<Texture> texture;
			int width;
			int height;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec4 color;
	};
}
#endif