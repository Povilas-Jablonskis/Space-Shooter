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
#include "ConfigurationManager.h"
#include "KeyBindingInputComponent.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	typedef std::pair<GLuint, std::vector<GLfloat>> cachedCharacter;

	class Text : public UIElementBase
	{
		public:
			Text(std::string, glm::vec4, glm::vec2, std::shared_ptr<UIInputComponent>);
			Text(std::string, glm::vec4, glm::vec2, std::shared_ptr<UIInputComponent>, std::shared_ptr<KeyBindingInputComponent>);
			void update(float, std::shared_ptr<ConfigurationManager>, std::shared_ptr<InputManager>);
			void fixPosition();
			bool checkIfCollides(glm::vec2);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			inline std::vector<cachedCharacter> getCachedCharacters() { return cachedCharacters; }
			inline std::string getText()  { return text; }
			inline void setText(std::string newtext) { needUpdate = true; text = newtext; }
			inline void setPosition(glm::vec2 _position) { RenderObject::setPosition(_position); needUpdate = true; }
			inline void setPosition(int index, float _position) { RenderObject::setPosition(index, _position); needUpdate = true; }
		private:
			bool needUpdate;
			std::string text;
			std::vector<cachedCharacter> cachedCharacters;
			SHORT leftButtonClicked;
			glm::vec4 bbox;
			std::shared_ptr<KeyBindingInputComponent> keyBindingInputComponent;
	};
}
#endif