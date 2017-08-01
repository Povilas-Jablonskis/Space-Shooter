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
			UIElement(int, int, glm::vec2, glm::vec4, std::shared_ptr<UIElement>, glm::vec2);
			inline void addText(std::shared_ptr<Text> text) { texts.push_back(text); }
			inline void addUIElement(std::shared_ptr<UIElement> uiElement) { elements.push_back(uiElement); }
			void hideMain(bool = false);
			void showMain(bool = false);
			void hideElement(size_t, bool = false);
			void showElement(size_t, bool = false);
			void fixPosition(UIElementBase* = nullptr);
			inline std::vector<std::shared_ptr<UIElement>>* getElements() { return &elements; }
			inline std::vector<std::shared_ptr<Text>>* getTexts() { return &texts; }
			inline void setParent(std::shared_ptr<UIElement> _parent) { parentMenu = _parent; }
			inline std::shared_ptr<UIElement> getParent() const { return parentMenu; }
			void checkIfMouseHoverThis(glm::vec2);
			void checkForMouseClickOnThis(std::shared_ptr<InputManager>, glm::vec2);
			void GetAllChildrenElements(std::vector<std::shared_ptr<UIElement>>*);
			void GetAllChildrenTexts(std::vector<std::shared_ptr<Text>>*);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
		private:
			std::shared_ptr<UIElement> parentMenu;
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
	};
}
#endif