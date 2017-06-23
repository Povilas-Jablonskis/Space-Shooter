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
			void Draw(GLuint);
			void DrawOtherObjects();
			virtual void Update();
			virtual std::vector<std::shared_ptr<BaseGameObject>>::iterator Update(std::vector<std::shared_ptr<BaseGameObject>>&, std::vector<std::shared_ptr<BaseGameObject>>::iterator);
			float GetPosition(int);
			int GetSize(int);
			float GetColor(int);
		protected:
			std::vector<std::shared_ptr<BaseGameObject>> objects;
			float position[2];
			float velocity[2];
			int width;
			int height;
			float color[3];
	};
}
#endif