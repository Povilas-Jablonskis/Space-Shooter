#ifndef _uIElementH
#define _uIElementH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "Text.h"

namespace Engine
{
	class UIElement : public UIElementBase
	{
		public:
			~UIElement();
			UIElement(int, int, glm::vec2, glm::vec4);
			UIElement();
			void AddText(std::shared_ptr<Text>);
			void AddUIElement(std::shared_ptr<UIElement>);
			void HideAllElements();
			void ShowAllElements();
			void Update(InputManager*);
			void Draw(InputManager*);
			void OnHoverEnterFuncDefaults();
			void OnHoverExitFuncDefaults();
			void OnMouseClickDefaults(InputManager*);
			void OnMouseReleaseFuncDefaults(InputManager*);
		private:
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
		};
}
#endif