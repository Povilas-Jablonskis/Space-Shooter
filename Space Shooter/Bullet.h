#ifndef _bulletH
#define _bulletH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Bullet.h"
#include "BaseGameObject.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			~Bullet();
			Bullet(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			inline glm::vec2 getVelocity() { return velocity; };
			void onCollision(BaseGameObject*);
	};
}
#endif