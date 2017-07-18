#ifndef _textureH
#define _textureH

#include <iostream>
#include <vector>
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
			GLuint getTexture() const;
			int getSize(int) const;
			void setCount(glm::vec2);
			int getStartFrame() const;
			int getEndFrame() const;
			glm::vec2 getCount() const;
			void setFrames(int, int);
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