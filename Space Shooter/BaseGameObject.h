#ifndef _BaseGameObject
#define _BaseGameObject

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Engine
{
	class BaseGameObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(int, int, float, float, float, float, float, float, float);
			BaseGameObject();
			void Draw(GLuint);
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