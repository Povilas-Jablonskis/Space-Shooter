#ifndef _effectManagerH
#define _effectManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

#include "BaseGameObject.h"
#include "SpriteSheetManager.h"
#include "irrKlang.h"

namespace Engine
{
	typedef std::pair<std::string, std::function<bool(std::shared_ptr<BaseGameObject>)>> effect;

	class EffectManager
	{
		public:
			void loadEffectsFromConfig(std::shared_ptr<SpriteSheetManager>, irrklang::ISoundEngine*);
			std::function<bool(std::shared_ptr<BaseGameObject>)> getEffect(std::string index);
			std::function<bool(std::shared_ptr<BaseGameObject>)> getRandomEffect();
		private:
			std::vector<effect> effects;
	};
}
#endif