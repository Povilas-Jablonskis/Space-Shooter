#include "Shader.hpp"

namespace Engine
{
	Shader::~Shader()
	{
		glDeleteProgram(m_program);
	}

	Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
	{
		loadShader(vertex_path, fragment_path);
	}

	std::string Shader::readShaderFile(const std::string& path) const
	{
		std::string content;
		std::ifstream fs("Shaders/" + path);
		char buffer;

		if (fs.is_open())
		{
			while (fs.get(buffer))
			{
				content.push_back(buffer);
			}
			fs.close();
		}
		else
		{
			#if _DEBUG
				std::cout << "ERROR::Shader: File " << path.c_str() << " not found\n";
				getchar();
			#endif
			exit(0);
		}
		return content;
	}

	void Shader::loadShader(const std::string& vertex_path, const std::string& fragment_path)
	{
		auto vertShaderStr = readShaderFile(vertex_path);
		auto fragShaderStr = readShaderFile(fragment_path);
		const char *vertexShaderSource = vertShaderStr.c_str();
		const char *fragmentShaderSource = fragShaderStr.c_str();
		GLint isCompiled = 0;

		// Vertex shader
		GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLogs(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLogs[0]);

			#if _DEBUG
				std::cout << "ERROR::Shader: Errors in vertex shader ( Shaders/" << vertex_path.c_str() << " ) :\n";
				for (auto errorLog : errorLogs)
				{
					std::cout << errorLog;
				}
			#endif
		}

		// Fragment shader
		GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLogs(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLogs[0]);

			#if _DEBUG
				std::cout << "ERROR::Shader: Errors in fragment shader ( Shaders/ " << fragment_path.c_str() << " ) :\n";
				for (auto errorLog : errorLogs)
				{
					std::cout << errorLog;
				}
			#endif
		}

		if (isCompiled == GL_FALSE)
		{
			#if _DEBUG
				getchar();
			#endif
			exit(0);
		}

		// Link shaders
		setShader(glCreateProgram());
		glAttachShader(m_program, vertexShader);
		glAttachShader(m_program, fragmentShader);
		glLinkProgram(m_program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}