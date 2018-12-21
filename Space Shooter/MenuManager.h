#ifndef _menuManagerH
#define _menuManagerH

#include "UIElementBase.h"

namespace Engine
{
	typedef std::pair<std::string, std::vector<std::shared_ptr<UIElementBase>>> menu;

	class MenuManager
	{
		public:
			std::vector<std::shared_ptr<menu>>* getMenus() { return &menus; }
			void addMenu(std::shared_ptr<menu> _menu) { menus.push_back(_menu); }
		private:
			std::vector<std::shared_ptr<menu>> menus;
	};
}
#endif