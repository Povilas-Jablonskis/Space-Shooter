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
	GLuint Application::VBO;
	GLuint Application::EBO;
	GLuint Application::VAO;

	GLuint Application::TextVBO;
	GLuint Application::TextTexture;

	Application::Application() 
		: fontManager(new FontManager()), gamestate(GameState::NOTSTARTEDYET)
	{
		glGenBuffers(1, &TextVBO);
		glGenTextures(1, &TextTexture);

		AddShader("shader", new Shader("shader.vert", "shader.frag"));
		AddShader("textshader", new Shader("textshader.vert", "textshader.frag"));

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)2);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	}

	void Application::AddShader(const std::string& name, Shader* shader)
	{
		if (shaders.find(name) != shaders.end())
			return;
		shaders.insert(std::pair<std::string, Shader*>(name, shader));
	}

	FT_Face* Application::LoadFont(const std::string& _path, const std::string& _name)
	{
		fontManager->LoadFont(_path, _name);
		return fontManager->GetFont(_name);
	}

	FT_Face* Application::GetFont(const std::string& _name)
	{
		return fontManager->GetFont(_name);
	}

	const GLuint Application::GetVAO()
	{
		return VAO;
	}

	const GLuint Application::GetShaderProgram(const std::string& name)
	{
		return shaders[name]->GetShader();
	}

	const GLuint Application::GetTextVBO()
	{
		return TextVBO;
	}

	const GLuint Application::GetTextTexture()
	{
		return TextTexture;
	}

	GameState Application::GetState() const
	{
		return gamestate;
	}

	bool Application::GetKey(char key) const
	{
		return pressedkeys[key];
	}

	void Application::SetKey(char key, bool boolean)
	{
		pressedkeys[key] = boolean;
	}

	void Application::SetKey(int key, bool boolean)
	{
		pressedkeys[key] = boolean;
	}

	void Application::SetState(GameState _state)
	{
		gamestate = _state;
	}

	Application::~Application()
	{

	}
}