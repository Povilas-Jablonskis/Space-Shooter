#include "Application.h"

namespace Engine
{
	Application::Application() 
		: renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), inputManager(std::make_shared<InputManager>()), textureManager(std::make_shared<TextureManager>()), gameState(GameState::NOTSTARTEDYET)
	{

	}

	void Application::addShader(const std::string& name, std::shared_ptr<Shader> shader)
	{
		renderer->addShader(name, shader);
	}

	void Application::loadFont(const std::string& _path, const std::string& _name)
	{
		fontManager->loadFont(_path, _name);
	}

	void Application::loadTexture(const std::string& _path, const std::string& _name, int _startFame, int _endFrame, glm::vec2 _animsc)
	{
		textureManager->loadTexture(_path, _name, _startFame, _endFrame, _animsc);
	}

	std::shared_ptr<Texture> Application::getTexture(const std::string& _name)
	{
		return textureManager->getTexture(_name);
	}

	Application::~Application()
	{

	}
}