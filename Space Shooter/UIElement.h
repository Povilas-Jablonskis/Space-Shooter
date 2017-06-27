#ifndef _UIElement
#define _UIElement

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

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
			void AddText(Text* text);
			void HideAllElements();
			void ShowAllElements();
			void Update();
			void DrawOtherElements();
		private:
			std::vector<UIElement*> elements;
			std::vector<Text*> texts;
		};
}
#endif