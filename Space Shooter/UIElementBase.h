#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>
#include <map>

#include "Texture.h"
#include "InputManager.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class UIElementBase
	{
		public:
			~UIElementBase();
			UIElementBase(int, int, glm::vec2, glm::vec4, glm::vec2);
			void initFuncs();
			void draw(GLuint);
			void update(float);
			inline void changeColor(float _color, int index) { color[index] = _color; }
			inline float getPosition(int index) const{ return position[index]; }
			int getSize(int) const;
			inline float getColor(int index) const{ return color[index]; }
			inline void setIsStatic(bool _isStatic) { isStatic = _isStatic; }
			inline std::shared_ptr<Texture> getTexture() { return texture; }
			void updateTexture(float);
			virtual void fixPosition(UIElementBase* = nullptr);
			void applyTexture(std::shared_ptr<Texture>);
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
			std::shared_ptr<Texture> texture;
			glm::vec2 position;
			int width;
			int height;
			glm::vec2 positionPercents;
			glm::vec4 color;
			bool isStatic;
			bool gotMousedHovered;
	};
}
#endif