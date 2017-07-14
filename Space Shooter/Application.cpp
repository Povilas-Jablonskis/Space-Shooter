#include "Application.h"

namespace Engine
{
	std::map<std::string, Shader*> Application::shaders;
	GLuint Application::indices[6] =
	{   // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};
	GLfloat Application::vertices[16]
	{
		// Positions    // Texture Coords
		1.0f, 1.0f, 1.0f, 1.0f, // Top Right
		1.0f, -1.0f, 1.0f, 0.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f, 0.0f, // Bottom Left
		-1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};
	GLuint Application::vbo;
	GLuint Application::ebo;
	GLuint Application::vao;

	GLuint Application::textVBO;
	GLuint Application::textTexture;

	Application::Application() 
		: fontManager(new FontManager()), inputManager(new InputManager()), textureManager(new TextureManager()), gameState(GameState::NOTSTARTEDYET)
	{
		glGenBuffers(1, &textVBO);
		glGenTextures(1, &textTexture);

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	}

	void Application::addShader(const std::string& name, Shader* shader)
	{
		if (shaders.find(name) != shaders.end())
			return;
		shaders.insert(std::pair<std::string, Shader*>(name, shader));
	}

	FT_Face* Application::loadFont(const std::string& _path, const std::string& _name)
	{
		fontManager->loadFont(_path, _name);
		return fontManager->getFont(_name);
	}

	FT_Face* Application::getFont(const std::string& _name)
	{
		return fontManager->getFont(_name);
	}

	const GLuint Application::getVAO()
	{
		return vao;
	}

	const GLuint Application::getShaderProgram(const std::string& name)
	{
		return shaders[name]->getShader();
	}

	const GLuint Application::getTextVBO()
	{
		return textVBO;
	}

	const GLuint Application::getTextTexture()
	{
		return textTexture;
	}

	GameState Application::getState() const
	{
		return gameState;
	}

	void Application::setState(GameState _state)
	{
		gameState = _state;
	}

	InputManager& Application::getInputManager()
	{
		return *inputManager;
	}

	void Application::loadTexture(const std::string& _path, const std::string& _name, glm::vec2 _animsc)
	{
		textureManager->loadTexture(_path, _name, _animsc);
	}

	Texture* Application::getTexture(const std::string& _name)
	{
		return textureManager->getTexture(_name);
	}

	Application::~Application()
	{

	}
}