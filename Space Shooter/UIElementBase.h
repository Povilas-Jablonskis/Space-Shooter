#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class UIElementBase
	{
		public:
			~UIElementBase();
			UIElementBase(int, int, glm::vec2, glm::vec4);
			UIElementBase();
			virtual void Draw();
			void ChangeColor(float, int);
			float GetPosition(int) const;
			int GetSize(int) const;
			float GetColor(int) const;
		protected:
			glm::vec2 position;
			int width;
			int height;
			glm::vec4 color;
		};
}
#endif