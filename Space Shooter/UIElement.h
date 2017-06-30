#ifndef _UIElement
#define _UIElement

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
			void AddText(const std::string&, int, glm::vec2, glm::vec4, FT_Face*);
			void AddText(std::shared_ptr<Text>);
			void AddUIElement(int, int, glm::vec2, glm::vec4);
			void AddUIElement(std::shared_ptr<UIElement>);
			void HideAllElements();
			void ShowAllElements();
			void Update();
			void Draw();
		private:
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
		};
}
#endif