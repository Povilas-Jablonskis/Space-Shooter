#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Application.h"

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
			virtual bool Update(float);
			virtual bool Update(float, float);
			virtual GLboolean CheckCollision(std::shared_ptr<BaseGameObject>);
			virtual GLboolean CheckCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
			void UpdateBulletList(float);
			void DrawBulletList();
			virtual void OnCollision(std::shared_ptr<BaseGameObject>);
			float GetPosition(int) const;
			glm::vec2 GetPosition() const;
			int GetSize(int) const;
			float GetColor(int) const;
		protected:
			int width;
			int height;
			glm::vec2 position;
			glm::vec2 velocity;
			glm::vec3 color;
			std::vector<std::shared_ptr<BaseGameObject>> bullets;
	};
}
#endif