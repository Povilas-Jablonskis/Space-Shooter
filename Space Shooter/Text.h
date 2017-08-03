#ifndef _textH
#define _textH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include "mat4x4.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include FT_FREETYPE_H
#include <functional>

#include "UIElementBase.h"
#include "InputManager.h"
#include "Font.h"
#include "StructForTextCache.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Text : public UIElementBase
	{
		public:
			Text(const std::string&, int, glm::vec2, glm::vec4, std::shared_ptr<Font>, glm::vec2);
			~Text();
			void update(float);
			inline std::shared_ptr<Font> getFont() const { return font; }
			inline std::vector<StructForTextCache> getCachedCharacters() { return cachedCharacters; }
			inline std::string getText() const { return text; }
			inline void setText(std::string newtext) { needUpdate = true; text = newtext; }
			inline void setLastText(std::string newtext) { lastText = newtext; }
			inline std::string getLastText() const { return lastText; }
			inline void setPosition(glm::vec2 _position) { needUpdate = true; position = _position; }
			inline void setPosition(int index, float _position) { needUpdate = true; position[index] = _position; }
			bool checkIfCollides(glm::vec2);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
		private:
			bool needUpdate;
			std::string lastText;
			std::string text;
			int fontSize;
			std::vector<StructForTextCache> cachedCharacters;
			std::shared_ptr<Font> font;
			SHORT leftButtonClicked;
	};
}
#endif