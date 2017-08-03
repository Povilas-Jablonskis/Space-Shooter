#ifndef _bulletH
#define _bulletH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Texture.h"
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
			Bullet(int, int, glm::vec2, glm::vec2, glm::vec4);
			~Bullet();
			bool update(float);
			void onCollision(BaseGameObject*, BaseGameObject*);
	};
}
#endif