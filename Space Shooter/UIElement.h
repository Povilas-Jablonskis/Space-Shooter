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
			UIElement(int, int, glm::vec2, glm::vec4, std::string, glm::vec2);
			inline void addText(std::shared_ptr<Text> text) { texts.push_back(text); }
			inline void addUIElement(std::shared_ptr<UIElement> uiElement) { elements.push_back(uiElement); }
			void hideAllElements();
			void showAllElements();
			void draw(GLuint, GLuint, GLuint, GLuint, GLuint);
			void update(InputManager*, float);
			void fixPosition(UIElementBase*);
			inline std::vector<std::shared_ptr<UIElement>>* getElements() { return &elements; }
			inline std::vector<std::shared_ptr<Text>>* getTexts() { return &texts; }
			inline void setParent(std::string _parent) { parentMenu = _parent; }
			inline std::string getParent() const { return parentMenu; }
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults(InputManager*);
			void onMouseReleaseFuncDefaults(InputManager*);
		private:
			std::string parentMenu;
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
		};
}
#endif