#ifndef textH
#define textH

#include <algorithm>
#include <vec2.hpp>
#include <vec4.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "UIElementBase.hpp"

namespace Engine
{
	class RenderObject;
	class ConfigurationManager;
	class InputManager;
	class KeyBindingInputComponent;

	typedef std::pair<GLuint, std::vector<GLfloat>> cachedCharacter;

	class Text : public UIElementBase
	{
		public:
			Text(const std::string&, const glm::vec4&, const glm::vec2&);
			Text(const std::string&, const glm::vec4&, const glm::vec2&, const std::shared_ptr<KeyBindingInputComponent>&);
			void update(float, const std::unique_ptr<ConfigurationManager>&, const std::unique_ptr<InputManager>&);
			void fixPosition();
			bool checkIfCollides(const glm::vec2&);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			inline const std::vector<cachedCharacter>& getCachedCharacters() const { return m_cachedCharacters; }
			inline const std::string& getText() const { return m_text; }
			inline bool doesItNeedUpdate() const { return m_needUpdate; }
			inline void setNeedUpdate(bool t_needUpdate) { m_needUpdate = t_needUpdate; }
			inline void setText(const std::string& newtext) { m_needUpdate = true; m_text = newtext; }
			void setPosition(const glm::vec2&);
			void setPosition(int, float);
		private:
			inline const glm::vec4& getBoundaryBox() const { return m_bbox; }
			inline const std::shared_ptr<KeyBindingInputComponent>& getKeyBindingInputComponent() const { return m_keyBindingInputComponent; }
			inline void changeBoundaryBox(float t_bbox, int index) { m_bbox[index] = t_bbox; }
			inline void changeBoundaryBox(glm::vec4& t_bbox) { m_bbox = t_bbox; }

			bool m_needUpdate{ true };
			std::string m_text;
			std::vector<cachedCharacter> m_cachedCharacters;
			SHORT m_leftButtonClicked{ 0 };
			glm::vec4 m_bbox;
			std::shared_ptr<KeyBindingInputComponent> m_keyBindingInputComponent;
	};
}
#endif