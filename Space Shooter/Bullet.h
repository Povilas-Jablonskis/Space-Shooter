#ifndef _bulletH
#define _bulletH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "BaseGameObject.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Player;
	class TestEnemy;
	class Bullet : public BaseGameObject
	{
		public:
			Bullet(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			void onCollision(Player*, TestEnemy*);
			void onCollision(TestEnemy*, Player*);
	};
}
#endif