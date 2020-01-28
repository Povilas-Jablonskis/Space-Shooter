#ifndef uIElementBaseH
#define uIElementBaseH

#include "RenderObject.hpp"

namespace Engine
{
	class InputManager;
	class UIInputComponent;

	class UIElementBase : public RenderObject
	{
		public:
			UIElementBase(const glm::vec4&, const glm::vec2&);
			void update(float, const std::shared_ptr<InputManager>&);
			virtual void fixPosition();
			virtual bool checkIfCollides(const glm::vec2&) const;
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
			const std::shared_ptr<UIInputComponent>& getUIInputComponent() const { return m_inputComponent; }
		private:
			bool m_active{ true };
			glm::vec2 m_positionPercents;
			bool m_gotMousedClicked{ false };
			bool m_gotMousedHovered{ false };
			std::shared_ptr<UIInputComponent> m_inputComponent{ std::make_shared<UIInputComponent>() };
	};
}
#endif