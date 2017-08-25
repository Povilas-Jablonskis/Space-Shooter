#ifndef _effectManagerH
#define _effectManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

namespace Engine
{
	template <class T>
	class EffectManager
	{
		public:
			void loadEffect(std::string index, std::function<void(T*)> _effect)
			{
				for (auto effect : effects)
				{
					if (effect.first == index)
						return;
				}

				effects.push_back(std::pair<std::string, std::function<void(T*)>>(index, _effect));
			}
			void loadEffect(std::pair<std::string, std::function<void(T*)>> _effect)
			{
				for (auto effect : effects)
				{
					if (effect.first == _effect.first)
						return;
				}

				effects.push_back(_effect);
			}
			std::function<void(T*)> getEffect(std::string index)
			{
				for (auto effect : effects)
				{
					if (effect.first == index)
						return effect.second;
				}

				return nullptr;
			}
			std::function<void(T*)> getRandomEffect()
			{
				int randIndex = rand() % effects.size();
				return effects[randIndex].second;
			}
		private:
			std::vector<std::pair<std::string, std::function<void(T*)>>> effects;
	};
}
#endif