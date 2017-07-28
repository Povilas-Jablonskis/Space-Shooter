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
			UIElement(int, int, glm::vec2, glm::vec4, std::shared_ptr<UIElement>, glm::vec2, std::shared_ptr<Application>);
			inline void addText(std::shared_ptr<Text> text) { texts.push_back(text); }
			inline void addUIElement(std::shared_ptr<UIElement> uiElement) { elements.push_back(uiElement); }
			void hideMain(bool = false);
			void showMain(bool = false);
			void hideElement(int, bool = false);
			void showElement(int, bool = false);
			void draw();
			void update(float);
			void fixPosition(UIElementBase*);
			inline std::vector<std::shared_ptr<UIElement>>* getElements() { return &elements; }
			inline std::vector<std::shared_ptr<Text>>* getTexts() { return &texts; }
			inline void setParent(std::shared_ptr<UIElement> _parent) { parentMenu = _parent; }
			inline std::shared_ptr<UIElement> getParent() const { return parentMenu; }
			void GetAllChildrenElements(std::vector<std::shared_ptr<UIElement>>*, std::shared_ptr<UIElement>);
			void GetAllChildrenTexts(std::vector<std::shared_ptr<Text>>*, std::shared_ptr<UIElement>);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults();
			void onMouseReleaseFuncDefaults();
		private:
			std::shared_ptr<UIElement> parentMenu;
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
	};
}
#endif