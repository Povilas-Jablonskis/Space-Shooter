#ifndef textH
#define textH

#include "InputManager.hpp"

#include <vector>
#include <functional>
#include <glew/glew.h>
#include <freeglut/freeglut_std.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Font;

typedef std::pair<GLuint, std::vector<GLfloat>> cachedCharacter;

class Text
{
public:
	Text(const std::string&, const glm::vec4&, const glm::vec2&, Font&);
	void update(const InputManager&);
	void fixPosition();
	void onHoverEnterFuncDefaults();
	void onHoverExitFuncDefaults();
	std::function<void()> onHoverEnterFunc;
	std::function<void()> onHoverExitFunc;
	std::function<void()> onMouseClickFunc;
	std::function<void()> onMouseReleaseFunc;
	const std::vector<cachedCharacter>& getCachedCharacters() const;
	const std::string& getText() const;
	const glm::vec4& getBoundingBox() const;
	bool doesItNeedUpdate() const;
	void setNeedUpdate(bool);
	void setText(const std::string&);
	void setPosition(const glm::vec2&);
	void setPosition(int, float);
	void disable();
	void enable();
	bool isActive() const;
	void setMousedHovered(bool);
	bool isHoveredByMouse() const;
	void setMousedClicked(bool);
	bool isClickedByMouse() const;
	const glm::vec2& getPositionPercents() const;
	void changeColor(float, int);
	void changeColor(const glm::vec4&);
	const glm::vec4& getColor() const;
	const glm::vec2& getPosition() const;
	float getRotationAngle() const;
	void setScale(float);
	void setRotationAngle(float);
	float getWidth() const;
	float getHeight() const;
	void setWidth(float);
	void setHeight(float);
	float getOriginalWidth() const;
	float getOriginalHeight() const;
	void setOriginalWidth(float);
	void setOriginalHeight(float);
private:
	bool m_active{ true };
	bool m_needUpdate{ true };
	Font& m_font;
	std::string m_text{};
	std::vector<cachedCharacter> m_cachedCharacters;
	glm::vec4 m_bbox{};
	glm::vec2 m_positionPercents{};
	bool m_gotMousedClicked{};
	bool m_gotMousedHovered{};
	float m_scale{ 0.5f };
	float m_rotationAngle{};
	glm::vec2 m_position{};
	float m_width{};
	float m_height{};
	glm::vec4 m_color{};
	float m_originalWidth{};
	float m_originalHeight{};
};
#endif