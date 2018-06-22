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
			void addShader(const std::string& name, std::shared_ptr<Shader> shader);
			inline std::vector<shader>* getShaders() { return &shaders; }
			inline GLuint getVAO() const { return vao; }
			GLuint getShaderProgram(const std::string&) const;
			inline GLuint getTextVBO() const { return textVBO; }
			inline GLuint getTextVAO() const { return textVAO; }
		private:
			GLuint vbo, vao, ebo;
			GLuint textVBO;
			GLuint textVAO;
			std::vector<shader> shaders;
	};
}
#endif