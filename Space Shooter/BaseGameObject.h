#ifndef _BaseGameObject
#define _BaseGameObject

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include <vec2.hpp>
#include <vec3.hpp>

namespace Engine
{
	class BaseGameObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(int, int, glm::vec2, glm::vec2, glm::vec3);
			BaseGameObject();
			virtual void Draw(GLuint);
			virtual bool Update() = 0;
			const float GetPosition(int);
			const int GetSize(int);
			const float GetColor(int);
		protected:
			int width;
			int height;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec3 color;
	};
}
#endif