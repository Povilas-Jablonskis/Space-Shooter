#include "Application.h"

namespace Engine
{
	Application::Application()
	{
		bulletManager = new BulletManager();
		fontLoader = new FontLoader();
		renderer = new Renderer();

		renderer->AddShader("shader", new Shader("shader.vert", "shader.frag"));
		renderer->AddShader("textshader", new Shader("textshader.vert", "textshader.frag"));
	}

	std::vector<std::shared_ptr<BaseGameObject>>& Application::GetBulletsList()
	{
		return bulletManager->GetBulletsList();
	}

	const Renderer& Application::GetRenderer()
	{
		return *renderer;
	}

	void Application::LoadFont(const std::string& fontpath, const std::string& fontname)
	{
		fontLoader->LoadFont(fontpath, fontname);
	}

	Application::~Application()
	{

	}
}