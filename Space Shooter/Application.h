#ifndef _Application
#define _Application

#include "BulletManager.h"
#include "FontLoader.h"
#include "Renderer.h"
#include "Bullet.h"

namespace Engine
{
	class Application
	{
		public:
			~Application();
			Application();
			std::vector<std::shared_ptr<BaseGameObject>>& GetBulletsList();
			const Renderer& GetRenderer();
			void LoadFont(const std::string& fontpath, const std::string& fontname);
		private:
			BulletManager* bulletManager;
			FontLoader* fontLoader;
			Renderer* renderer;
		};
}
#endif