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
			UIElement(int, int, float, float, float, float, float, float);
			UIElement();
			void AddText(std::string, int, float, float, float, float, float, float, std::string);
			void AddText(std::shared_ptr<Text>);
			void AddUIElement(int, int, float, float, float, float, float, float);
			void AddUIElement(std::shared_ptr<UIElement>);
			void HideAllElements();
			void ShowAllElements();
			void Update();
			void DrawOtherElements();
		private:
			std::vector<std::shared_ptr<UIElement>> elements;
			std::vector<std::shared_ptr<Text>> texts;
		};
}
#endif