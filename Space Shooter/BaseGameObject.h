#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "RenderObject.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class BaseGameObject : public RenderObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(int, int, glm::vec2, glm::vec2, glm::vec4);
			virtual bool update(float);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			inline glm::vec2 getVelocity() { return velocity; };
			void addBullet(std::shared_ptr<BaseGameObject>, std::shared_ptr<Texture> = nullptr);
			inline std::vector<std::shared_ptr<BaseGameObject>>* getBulletsList() { return &bullets; }
			virtual void onCollision(BaseGameObject*);
		protected:
			std::vector<std::shared_ptr<BaseGameObject>> bullets;
			glm::vec2 velocity;
	};
}
#endif