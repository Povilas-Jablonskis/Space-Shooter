#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <freeglut/freeglut.h>

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
	loadShader(vertex_path, fragment_path);
}

std::string Shader::readShaderFile(const std::string& path)
{
	std::string content;
	std::ifstream fs("assets/Shaders/" + path);

	if (fs.is_open())
	{
		char buffer;
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
		(void)getchar();
#endif
		glutLeaveMainLoop();
	}
	return content;
}

GLuint Shader::getShader() const
{
	return m_program;
}

void Shader::loadShader(const std::string& vertex_path, const std::string& fragment_path)
{
	const auto vertShaderStr = readShaderFile(vertex_path);
	const auto fragShaderStr = readShaderFile(fragment_path);
	const auto vertexShaderSource = vertShaderStr.c_str();
	const auto fragmentShaderSource = fragShaderStr.c_str();
	auto isCompiled = 0;

	// Vertex shader
	const GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		auto maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLogs(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, errorLogs.data());

#if _DEBUG
		std::cout << "ERROR::Shader: Errors in vertex shader ( Shaders/" << vertex_path.c_str() << " ) :\n";
		for (const auto errorLog : errorLogs)
		{
			std::cout << errorLog;
		}
#endif
	}

	// Fragment shader
	const GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		auto maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLogs(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, errorLogs.data());

#if _DEBUG
		std::cout << "ERROR::Shader: Errors in fragment shader ( Shaders/ " << fragment_path.c_str() << " ) :\n";
		for (const auto errorLog : errorLogs)
		{
			std::cout << errorLog;
		}
#endif
	}

	if (isCompiled == GL_FALSE)
	{
#if _DEBUG
		(void)getchar();
#endif
		glutLeaveMainLoop();
	}

	// Link shaders
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
