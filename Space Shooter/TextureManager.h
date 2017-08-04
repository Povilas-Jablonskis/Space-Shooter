#ifndef _textureManagerH
#define _textureManagerH

#include <iostream>
#include <unordered_map>
#include <memory>
#include <vec2.hpp>

#include "Texture.h"

namespace Engine
{
	class TextureManager
	{
		public:
			~TextureManager();
			void loadTexture(const std::string&, const std::string&, int, int, glm::vec2);
			std::shared_ptr<Texture> getTexture(const std::string&);
		private:
			std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}
#endif