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
			Texture();
			~Texture();
			void readTextureFile(const std::string&);
			GLuint getTexture() const;
			int getSize(int) const;
			void setCount(glm::vec2);
			glm::vec2 getCount() const;
			void setCurrentFrame(int);
			int getCurrentFrame() const;
			void setFrames(int, int);
			void setDelay(float);
			void setAnimationStatus(bool);
			void setLoopStatus(bool);
			void update(float);
		private:
			bool animComplete;
			float animTimer;
			bool loop;
			bool animationDone;
			float delay;
			int endFrame;
			int startFrame;
			glm::vec2 animsc;
			int currentFrame;
			int width;
			int height;
			GLuint texture;
	};
}
#endif