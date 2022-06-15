#ifndef textH
#define textH

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <vector>

#include "UIElementBase.hpp"

class Font;
class InputManager;

typedef std::pair<GLuint, std::vector<GLfloat>> cachedCharacter;

class Text final : public UIElementBase
{
public:
	Text(std::string, const glm::vec4&, const glm::vec2&);
	void update(const std::shared_ptr<InputManager>&);
	void fixPosition() override;
	void onHoverEnterFuncDefaults() override;
	void onHoverExitFuncDefaults() override;
	const std::vector<cachedCharacter>& getCachedCharacters() const { return m_cachedCharacters; }
	const std::string& getText() const { return m_text; }
	const glm::vec4& getBoundingBox() const { return m_bbox; }
	bool doesItNeedUpdate() const { return m_needUpdate; }
	void setNeedUpdate(bool t_needUpdate) { m_needUpdate = t_needUpdate; }
	void setText(const std::string& newtext) { m_needUpdate = true; m_text = newtext; }
	void setPosition(const glm::vec2&) override;
	void setPosition(int, float) override;
private:

	bool m_needUpdate{ true };
	std::shared_ptr<Font> m_font;
	std::string m_text;
	std::vector<cachedCharacter> m_cachedCharacters;
	glm::vec4 m_bbox = { 0.0f, 0.0f, 0.0f, 0.0f };
};
#endif