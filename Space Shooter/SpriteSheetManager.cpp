#include "SpriteSheetManager.h"

namespace Engine
{
	SpriteSheetManager::~SpriteSheetManager()
	{
		spriteSheets.clear();
	}

	void SpriteSheetManager::loadSpriteSheet(const std::string& name, std::shared_ptr<SpriteSheet> spriteSheet)
	{
		if (spriteSheets.find(name) == spriteSheets.end())
		{
			spriteSheets.insert(std::pair<std::string, std::shared_ptr<SpriteSheet>>(name, spriteSheet));
		}
	}

	std::shared_ptr<SpriteSheet> SpriteSheetManager::getSpriteSheet(const std::string& name)
	{
		auto spriteSheet = spriteSheets.find(name);
		if (spriteSheet == spriteSheets.end())
			return nullptr;

		return spriteSheet->second;
	}
}