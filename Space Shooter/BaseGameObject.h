#ifndef _baseGameObjectH
#define _baseGameObjectH

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
			virtual void Draw();
			virtual bool Update();
			float GetPosition(int) const;
			int GetSize(int) const;
			float GetColor(int) const;
		protected:
			int width;
			int height;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec3 color;
	};
}
#endif