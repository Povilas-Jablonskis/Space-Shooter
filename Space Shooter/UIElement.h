#ifndef _UIElement
#define _UIElement

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "UIElementBase.h"
#include "Text.h"

namespace Engine
{
	class UIElement : public UIElementBase
	{
		public:
			~UIElement();
			UIElement(int, int, float, float, float, float, float);
			UIElement();
			void AddText(const char*, int, float, float, float, float, float, const char* font, UIElementBase*);
			void AddText(Text* text);
			void Update();
			void DrawOther();
			std::vector<UIElementBase*> elements;
			std::vector<Text*> texts;
		};
}
#endif