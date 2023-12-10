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
	Text(std::string text, const glm::vec4& color, const glm::vec2& positionPercents, Font& font);
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
	void setNeedUpdate(bool needUpdate);
	void setText(const std::string& text);
	void setPosition(const glm::vec2& position);
	void setPosition(const int index, const float position);
	void disable();
	void enable();
	[[nodiscard]] bool isActive() const;
	void setMousedHovered(const bool gotMousedHovered);
	[[nodiscard]] bool isHoveredByMouse() const;
	void setMousedClicked(const bool gotMousedClicked);
	[[nodiscard]] bool isClickedByMouse() const;
	[[nodiscard]] const glm::vec2& getPositionPercents() const;
	void changeColor(const glm::vec4& color);
	[[nodiscard]] const glm::vec4& getColor() const;
	[[nodiscard]] const glm::vec2& getPosition() const;
	[[nodiscard]] float getRotationAngle() const;
	void setScale(const float scale);
	void setRotationAngle(const float rotation);
	[[nodiscard]] float getWidth() const;
	[[nodiscard]] float getHeight() const;
	void setWidth(const float width);
	void setHeight(const float height);
	[[nodiscard]] float getOriginalWidth() const;
	[[nodiscard]] float getOriginalHeight() const;
	void setOriginalWidth(const float width);
	void setOriginalHeight(const float height);

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
