#include "Shader.hpp"

#include <fstream>
#include <iostream>

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
	loadShader(vertex_path, fragment_path);
}

std::string Shader::readShaderFile(const std::string& path) const
{
	std::string content;
	std::ifstream fs("assets/Shaders/" + path);
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
		(void)getchar();
#endif
		exit(0);
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
	auto vertexShaderSource = vertShaderStr.c_str();
	auto fragmentShaderSource = fragShaderStr.c_str();
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
		(void)getchar();
#endif
		exit(0);
	}

	// Link shaders
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
