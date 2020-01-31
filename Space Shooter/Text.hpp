#ifndef textH
#define textH

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <vector>

#include "UIElementBase.hpp"

namespace Engine
{
	class RenderObject;
	class ConfigurationManager;
	class InputManager;

	typedef std::pair<GLuint, std::vector<GLfloat>> cachedCharacter;

	class Text final : public UIElementBase
	{
		public:
			Text(std::string, const glm::vec4&, const glm::vec2&);
			void update(const std::shared_ptr<ConfigurationManager>&, const std::shared_ptr<InputManager>&);
			void fixPosition() override;
			bool checkIfCollides(const glm::vec2&) const override;
			void onHoverEnterFuncDefaults() override;
			void onHoverExitFuncDefaults() override;
			const std::vector<cachedCharacter>& getCachedCharacters() const { return m_cachedCharacters; }
			const std::string& getText() const { return m_text; }
			bool doesItNeedUpdate() const { return m_needUpdate; }
			void setNeedUpdate(bool t_needUpdate) { m_needUpdate = t_needUpdate; }
			void setText(const std::string& newtext) { m_needUpdate = true; m_text = newtext; }
			void setPosition(const glm::vec2&) override;
			void setPosition(int, float) override;
		private:
			const glm::vec4& getBoundaryBox() const { return m_bbox; }
			void changeBoundaryBox(float t_bbox, int index) { m_bbox[index] = t_bbox; }
			void changeBoundaryBox(glm::vec4& t_bbox) { m_bbox = t_bbox; }

			bool m_needUpdate{ true };
			std::string m_text;
			std::vector<cachedCharacter> m_cachedCharacters;
			glm::vec4 m_bbox = {0.0f, 0.0f, 0.0f, 0.0f };
	};
}
#endif