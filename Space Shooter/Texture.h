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
		private: 
			int endFrame;
			int startFrame;
			glm::vec2 animsc;
			int width;
			int height;
			GLuint texture;
	};
}
#endif