#ifndef _Player
#define _Player

#include "BaseGameObject.h"
#include <map>
#include "UIElement.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			~Player();
			Player(int, int, float, float, float, float, float, float, float);
			Player();
			void Update();
			void DrawUI();
		private:
			void InitUI();
			bool pressedkeys[256];
			int score;
			int health;
			std::map<std::string, std::shared_ptr<UIElement>> UI;
	};
}
#endif