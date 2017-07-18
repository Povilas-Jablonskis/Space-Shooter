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
			UIElement(int, int, glm::vec2, glm::vec4, UIElement*, glm::vec2);
			void addText(std::shared_ptr<Text>);
			void addUIElement(std::shared_ptr<UIElement>);
			void hideAllElements();
			void showAllElements();
			void update(InputManager*, float);
			void draw();
			void fixPosition(UIElementBase*);
			void setParent(UIElement*);
			UIElement* getParent();
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults(InputManager*);
			void onMouseReleaseFuncDefaults(InputManager*);
		private:
			UIElement* parent;
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
		};
}
#endif