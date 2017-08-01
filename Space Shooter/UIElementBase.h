#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>
#include <map>

#include "RenderObject.h"
#include "InputManager.h"

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
			~UIElementBase();
			UIElementBase(int, int, glm::vec2, glm::vec4, glm::vec2);
			void initFuncs();
			virtual void update(float);
			inline void setIsStatic(bool _isStatic) { isStatic = _isStatic; }
			virtual void fixPosition(UIElementBase* = nullptr);
			virtual bool checkIfCollides(glm::vec2);
			virtual void checkIfMouseHoverThis(glm::vec2);
			virtual void checkForMouseClickOnThis(std::shared_ptr<InputManager>, glm::vec2);
			std::function<void()> onHoverEnterFunc;
			std::function<void()> onHoverExitFunc;
			std::function<void()> onMouseClickFunc;
			std::function<void()> onMouseReleaseFunc;
			virtual void onHoverEnterFuncDefaults();
			virtual void onHoverExitFuncDefaults();
		protected:
			glm::vec2 positionPercents;
			bool isStatic;
			bool gotMousedHovered;
	};
}
#endif