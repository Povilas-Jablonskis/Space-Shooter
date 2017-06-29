#ifndef _BaseGameObject
#define _BaseGameObject

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

namespace Engine
{
	class BaseGameObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(int, int, float, float, float, float, float, float, float);
			BaseGameObject();
			virtual void Draw(GLuint);
			virtual void Update();
			float GetPosition(int);
			int GetSize(int);
			float GetColor(int);
		protected:
			float position[2];
			float velocity[2];
			int width;
			int height;
			float color[3];
	};
}
#endif