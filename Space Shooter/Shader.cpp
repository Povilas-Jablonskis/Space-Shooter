#include "Shader.h"

namespace Engine
{
	Shader::Shader(std::string vertex_path, std::string fragment_path)
	{
		LoadShader(vertex_path, fragment_path);
	}

	Shader::Shader()
	{

	}

	Shader::~Shader()
	{

	}

	GLuint Shader::GetShader()
	{
		return program;
	}

	std::string Shader::ReadShaderFile(std::string path)
	{
		std::string content;
		FILE * pFile;
		char buffer;

		fopen_s(&pFile, path.c_str(), "rb");
		if (pFile == NULL) {
			#if _DEBUG
				std::cout << "File " << path.c_str() << " not found" << std::endl;
				getchar();
			#endif
			exit(0);
		} 
		else
		{
			while (true)
			{
				buffer = fgetc(pFile);
				if (buffer == EOF)
					break;
				content.push_back(buffer);
			}
			fclose(pFile);
		}
		return content;
	}

	void Shader::LoadShader(std::string vertex_path, std::string fragment_path)
	{
		std::string vertShaderStr = ReadShaderFile(vertex_path);
		std::string fragShaderStr = ReadShaderFile(fragment_path);
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
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

			for (size_t i = 0; i < errorLog.size(); i++)
				std::cout << errorLog[i];
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
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

			for (size_t i = 0; i < errorLog.size(); i++)
				std::cout << errorLog[i];
		}

		if (isCompiled == GL_FALSE)
		{
			#if _DEBUG
				getchar();
			#endif
			exit(0);
		}

		// Link shaders
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}