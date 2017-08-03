#ifndef _bulletH
#define _bulletH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Texture.h"
#include "RenderObject.h"
#include "CollisionType.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Bullet : public RenderObject
	{
		public:
			Bullet(int, int, glm::vec2, glm::vec2, glm::vec4, RenderObject*);
			~Bullet();
			bool update(float);
			void onCollision(RenderObject*, RenderObject*, CollisionType);
		private:
			RenderObject* parent;
	};
}
#endif