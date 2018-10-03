#ifndef _rendererH
#define _rendererH

#include <memory>
#include <algorithm>

#include "Shader.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Shader>> shader;

	class Renderer
	{
		public:
			~Renderer();
			Renderer();
			void addShader(std::string name, std::shared_ptr<Shader> shader);
			inline std::vector<shader>* getShaders() { return &shaders; }
			inline GLuint getVAO()  { return vao; }
			GLuint getShaderProgram(std::string);
			inline GLuint getTextVBO()  { return textVBO; }
			inline GLuint getTextVAO()  { return textVAO; }
		private:
			GLuint vbo, vao, ebo;
			GLuint textVBO;
			GLuint textVAO;
			std::vector<shader> shaders;
	};
}
#endif