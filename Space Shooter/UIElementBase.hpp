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
			void update(float, const std::unique_ptr<InputManager>&);
			virtual void fixPosition();
			virtual bool checkIfCollides(const glm::vec2&) const;
			std::function<void()> onHoverEnterFunc;
			std::function<void()> onHoverExitFunc;
			std::function<void()> onMouseClickFunc;
			std::function<void()> onMouseReleaseFunc;
			virtual void onHoverEnterFuncDefaults();
			virtual void onHoverExitFuncDefaults();
			inline void disable() { m_active = false; }
			inline void enable() { m_active = true; }
			inline bool isActive() const { return m_active; }
			inline void setMousedHovered(bool boolean) { m_gotMousedHovered = boolean; }
			inline bool isHoveredByMouse() const { return m_gotMousedHovered; }
			inline void setMousedClicked(bool boolean) { m_gotMousedClicked = boolean; }
			inline bool isClickedByMouse() const { return m_gotMousedClicked; }
			inline const glm::vec2& getPositionPercents() const { return m_positionPercents; }
			inline const std::shared_ptr<UIInputComponent>& getUIInputComponent() const { return m_inputComponent; }
		private:
			bool m_active{ true };
			glm::vec2 m_positionPercents;
			bool m_gotMousedClicked{ false };
			bool m_gotMousedHovered{ false };
			std::shared_ptr<UIInputComponent> m_inputComponent{ std::make_shared<UIInputComponent>() };
	};
}
#endif