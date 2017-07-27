#ifndef _uIElementBaseH
#define _uIElementBaseH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>
#include <map>

#include "Application.h"

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
			UIElementBase(int, int, glm::vec2, glm::vec4, glm::vec2, std::shared_ptr<Application>);
			void initFuncs();
			virtual void draw();
			virtual void update(float);
			inline void changeColor(float _color, int index) { color[index] = _color; }
			inline float getPosition(int index) const{ return position[index]; }
			int getSize(int) const;
			inline float getColor(int index) const{ return color[index]; }
			inline void setCurrentFrame(int frame) { currentFrame = frame; }
			inline int getCurrentFrame() const { return currentFrame; }
			inline void setDelay(float _delay) { delay = _delay; }
			inline void setAnimationStatus(bool _status) { animComplete = _status; }
			inline void setLoopStatus(bool _status) { loop = _status; }
			void updateTexture(float);
			virtual void fixPosition(UIElementBase*);
			void applyTexture(const std::string&);
			void setCurrentTextureCurrentFrame(int);
			bool checkForMouseCollision();
			std::function<void()> onHoverEnterFunc;
			std::function<void()> onHoverExitFunc;
			std::function<void()> onMouseClickFunc;
			std::function<void()> onMouseReleaseFunc;
			virtual void onHoverEnterFuncDefaults() = 0;
			virtual void onHoverExitFuncDefaults() = 0;
			void onMouseClickDefaults();
			void onMouseReleaseFuncDefaults();
		protected:
			std::shared_ptr<Application> application;
			bool animComplete;
			float animTimer;
			bool loop;
			float delay;
			int currentFrame;
			std::string texture;
			glm::vec2 position;
			int width;
			int height;
			glm::vec2 positionPercents;
			glm::vec4 color;
	};
}
#endif