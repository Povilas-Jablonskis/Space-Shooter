#ifndef _menuH
#define _menuH

#include "Text.h"

namespace Engine
{
	class Menu
	{
		public:
			inline void addText(std::shared_ptr<Text> text) { texts.push_back(text); }
			inline void addUIElement(std::shared_ptr<UIElementBase> uiElement) { elements.push_back(uiElement); }
			inline std::vector<std::shared_ptr<UIElementBase>>& getElements() { return elements; }
			inline std::vector<std::shared_ptr<Text>>& getTexts() { return texts; }
		private:
			std::vector<std::shared_ptr<UIElementBase>> elements;
			std::vector<std::shared_ptr<Text>> texts;
	};
}
#endif