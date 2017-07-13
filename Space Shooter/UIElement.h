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
			void addText(std::shared_ptr<Text>);
			void addUIElement(std::shared_ptr<UIElement>);
			void hideAllElements();
			void showAllElements();
			void update(InputManager*, float);
			void draw();
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults(InputManager*);
			void onMouseReleaseFuncDefaults(InputManager*);
		private:
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
		};
}
#endif