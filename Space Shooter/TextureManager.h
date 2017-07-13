#ifndef _textureManagerH
#define _textureManagerH

#include <iostream>
#include <map>
#include <vec2.hpp>

#include "Texture.h"

namespace Engine
{
	class TextureManager
	{
		public:
			TextureManager();
			~TextureManager();
			void loadTexture(const std::string&, const std::string&, glm::vec2);
			Texture getTexture(const std::string&);
			void setAnimationCount(const std::string&, glm::vec2);
		private:
			std::map<std::string, Texture> textures;
	};
}
#endif