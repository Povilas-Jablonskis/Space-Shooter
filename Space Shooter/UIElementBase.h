#ifndef _UIElementBase
#define _UIElementBase

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Engine
{
	class UIElementBase
	{
		public:
			~UIElementBase();
			UIElementBase(int, int, float, float, float, float, float);
			UIElementBase();
			virtual void Draw(GLuint);
			virtual void DrawOther();
			virtual void Update();
			float GetPosition(int);
			int GetSize(int);
			float GetColor(int);
		protected:
			float position[2];
			int width;
			int height;
			float color[3];
		};
}
#endif