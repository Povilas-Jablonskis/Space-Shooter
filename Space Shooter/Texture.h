#ifndef _textureH
#define _textureH

#include <iostream>
#include <vector>
#include <map>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "vec2.hpp"

namespace Engine
{
	class Texture
	{
		public:
			Texture(int, int, glm::vec2);
			~Texture();
			void readTextureFile(const std::string&);
			inline GLuint getTexture() const { return texture; }
			int getSize(int index) const;
			inline void setCount(glm::vec2 count) { animsc = count; }
			inline int getStartFrame() const { return startFrame; }
			inline int getEndFrame() const { return endFrame; }
			inline glm::vec2 getCount() const { return animsc; }
			inline void setFrames(int _startFrame, int _endFrame)
			{ 
				startFrame = _startFrame;
				endFrame = _endFrame;
			}
			inline void setCurrentFrame(int frame) { currentFrame = frame; }
			inline int getCurrentFrame() const { return currentFrame; }
			inline void setDelay(float _delay) { delay = _delay; }
			inline float getDelay() const { return delay; }
			inline void setAnimationStatus(bool _status) { animComplete = _status; }
			inline bool getAnimationStatus() const { return animComplete; }
			inline void setAnimTimer(float _timer) { animTimer = _timer; }
			inline float getAnimTimer() const { return animTimer; }
			inline void setLoopStatus(bool _status) { loop = _status; }
			inline bool getLoopStatus() const { return loop; }
		private:
			bool animComplete;
			float animTimer;
			bool loop;
			float delay;
			int currentFrame;
			int endFrame;
			int startFrame;
			glm::vec2 animsc;
			int width;
			int height;
			GLuint texture;
	};
}
#endif