#ifndef textH
#define textH

#include <vector>
#include <functional>
#include <glew/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Font.hpp"

using CachedCharacter = std::pair<GLuint, std::vector<GLfloat>>;

class Text
{
public:
	Text(std::string, const glm::vec4&, const glm::vec2&, Font&);
	void update();
	void updatePosition();
	void onHoverEnterFuncDefaults();
	void onHoverExitFuncDefaults();
	std::function<void()> onHoverEnterFunc;
	std::function<void()> onHoverExitFunc;
	std::function<void()> onMouseClickFunc;
	std::function<void()> onMouseReleaseFunc;
	[[nodiscard]] const std::vector<CachedCharacter>& getCachedCharacters() const;
	[[nodiscard]] const std::string& getText() const;
	[[nodiscard]] const glm::vec4& getBoundingBox() const;
	[[nodiscard]] bool doesNeedUpdate() const;
	void setNeedUpdate(bool);
	void setText(const std::string&);
	void setPosition(const glm::vec2&);
	void setPosition(int, float);
	void disable();
	void enable();
	[[nodiscard]] bool isActive() const;
	void setMousedHovered(bool);
	[[nodiscard]] bool isHoveredByMouse() const;
	void setMousedClicked(bool);
	[[nodiscard]] bool isClickedByMouse() const;
	[[nodiscard]] const glm::vec2& getPositionPercents() const;
	void changeColor(const glm::vec4&);
	[[nodiscard]] const glm::vec4& getColor() const;
	[[nodiscard]] const glm::vec2& getPosition() const;
	[[nodiscard]] float getRotationAngle() const;
	void setScale(float);
	void setRotationAngle(float);
	[[nodiscard]] float getWidth() const;
	[[nodiscard]] float getHeight() const;
	void setWidth(float);
	void setHeight(float);
	[[nodiscard]] float getOriginalWidth() const;
	[[nodiscard]] float getOriginalHeight() const;
	void setOriginalWidth(float);
	void setOriginalHeight(float);

private:
	bool m_active{true};
	bool m_needUpdate{true};
	Font& m_font;
	std::string m_text{};
	std::vector<CachedCharacter> m_cachedCharacters;
	glm::vec4 m_bbox{};
	glm::vec2 m_positionPercents{};
	bool m_gotMousedClicked{};
	bool m_gotMousedHovered{};
	float m_scale{0.5f};
	float m_rotationAngle{};
	glm::vec2 m_position{};
	float m_width{};
	float m_height{};
	glm::vec4 m_color{};
	float m_originalWidth{};
	float m_originalHeight{};
};
#endif
