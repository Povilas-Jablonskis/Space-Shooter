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
			inline void setDelay(float _delay) { delay = _delay; }
			inline float getDelay() const { return delay; }
			inline void setLoopStatus(bool _status) { loop = _status; }
			inline bool getLoopStatus() const { return loop; }
		private:
			bool loop;
			float delay;
			int endFrame;
			int startFrame;
			glm::vec2 animsc;
			int width;
			int height;
			GLuint texture;
	};
}
#endif