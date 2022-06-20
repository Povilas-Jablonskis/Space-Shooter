#ifndef uIElementBaseH
#define uIElementBaseH

#include "RenderObject.hpp"

class InputManager;

class UIElementBase : public RenderObject
{
public:
	UIElementBase(const glm::vec4&, const glm::vec2&);
	void update(float, const std::shared_ptr<InputManager>&);
	virtual void fixPosition();
	std::function<void()> onHoverEnterFunc;
	std::function<void()> onHoverExitFunc;
	std::function<void()> onMouseClickFunc;
	std::function<void()> onMouseReleaseFunc;
	virtual void onHoverEnterFuncDefaults();
	virtual void onHoverExitFuncDefaults();
	void disable() { m_active = false; }
	void enable() { m_active = true; }
	bool isActive() const { return m_active; }
	void setMousedHovered(const bool boolean) { m_gotMousedHovered = boolean; }
	bool isHoveredByMouse() const { return m_gotMousedHovered; }
	void setMousedClicked(const bool boolean) { m_gotMousedClicked = boolean; }
	bool isClickedByMouse() const { return m_gotMousedClicked; }
	const glm::vec2& getPositionPercents() const { return m_positionPercents; }
private:
	bool m_active{ true };
	glm::vec2 m_positionPercents{};
	bool m_gotMousedClicked{};
	bool m_gotMousedHovered{};
};
#endif