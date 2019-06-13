#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>

#include "RenderObject.h"
#include "UIInputComponent.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class UIElementBase : public RenderObject
	{
		public:
			UIElementBase(glm::vec4, glm::vec2, std::shared_ptr<UIInputComponent>);
			virtual void update(float, std::shared_ptr<InputManager>);
			virtual void fixPosition();
			virtual bool checkIfCollides(glm::vec2);
			std::function<void()> onHoverEnterFunc;
			std::function<void()> onHoverExitFunc;
			std::function<void()> onMouseClickFunc;
			std::function<void()> onMouseReleaseFunc;
			virtual void onHoverEnterFuncDefaults();
			virtual void onHoverExitFuncDefaults();
			inline void disable() { active = false; }
			inline void enable() { active = true; }
			inline bool isActive() { return active; }
			inline void setMousedHovered(bool boolean) { gotMousedHovered = boolean; }
			inline bool isMousedHovered() { return gotMousedHovered; }
			inline glm::vec2 getPositionPercents() { return positionPercents; }
			inline std::shared_ptr<UIInputComponent> getUIInputComponent() { return inputComponent; }
		private:
			bool active;
			glm::vec2 positionPercents;
			bool gotMousedHovered;
			std::shared_ptr<UIInputComponent> inputComponent;
	};
}
#endif