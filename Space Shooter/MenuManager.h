#ifndef _menuManagerH
#define _menuManagerH

#include "Menu.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Menu>> menu;

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