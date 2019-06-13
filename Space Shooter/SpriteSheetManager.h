#ifndef _spriteSheetManagerH
#define _spriteSheetManagerH

#include "SpriteSheet.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<SpriteSheet>> spriteSheet;

	class SpriteSheetManager
	{
		public:
			void loadSpriteSheetsFromConfig();
			std::shared_ptr<SpriteSheet> getSpriteSheet(std::string);
		private:
			void loadSpriteSheet(std::string, std::shared_ptr<SpriteSheet>);
			std::vector<spriteSheet> spriteSheets;
	};
}
#endif