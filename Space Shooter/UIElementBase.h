#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>
#include <map>

#include <vec2.hpp>
#include <vec4.hpp>

#include "InputManager.h"

namespace Engine
{
	class UIElementBase
	{
		public:
			~UIElementBase();
			UIElementBase(int, int, glm::vec2, glm::vec4);
			UIElementBase();
			void InitFuncs();
			virtual void Draw(InputManager*);
			void ChangeColor(float, int);
			float GetPosition(int) const;
			int GetSize(int) const;
			float GetColor(int) const;
			bool CheckForMouseCollision(InputManager*);
			std::function<void()> OnHoverEnterFunc;
			std::function<void()> OnHoverExitFunc;
			std::function<void()> OnMouseClickFunc;
			std::function<void()> OnMouseReleaseFunc;
			virtual void OnHoverEnterFuncDefaults() = 0;
			virtual void OnHoverExitFuncDefaults() = 0;
			void OnMouseClickDefaults(InputManager*);
			void OnMouseReleaseFuncDefaults(InputManager*);
		protected:
			glm::vec2 position;
			int width;
			int height;
			glm::vec4 color;
		};
}
#endif