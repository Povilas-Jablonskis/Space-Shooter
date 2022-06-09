#ifndef levelManagerH
#define levelManagerH

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <irrKlang/irrKlang.h>
#include <rapidxml/rapidxml.hpp>
#include <vector>
#include <iostream>

class UIElementBase;
class Text;
class BaseGameObject;
class InputManager;
class GameStateManager;
class SpriteSheetManager;
class Player;
class Entity;
class Renderer;
class UIManager;

class LevelManager
{
public:
	LevelManager(const std::shared_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*, int);
	void addExplosion(const std::shared_ptr<BaseGameObject>& _explosion) { m_explosions.push_back(_explosion); }
	void update(float, const std::shared_ptr<GameStateManager>&, const std::shared_ptr<InputManager>&, const std::shared_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*);
	void render(float, const std::shared_ptr<GameStateManager>&, const std::shared_ptr<InputManager>&, const std::shared_ptr<Renderer>&, const std::shared_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*);
private:
	int getCurrentLevel() const { return m_currentLevel; }
	void loadLevel(const std::shared_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*);
	const std::shared_ptr<UIManager>& getUIManager() const { return m_uiManager; }

	std::shared_ptr<Player> m_player{ nullptr };

	float m_currentTime{ static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) };
	float m_accumulator{ 0.0f };
	int m_maxLevels{ 0 };
	int m_currentLevel{ 0 };

	std::shared_ptr<UIManager> m_uiManager{ std::make_shared<UIManager>() };

	std::vector<std::shared_ptr<BaseGameObject>> m_meteors;
	std::vector<std::shared_ptr<BaseGameObject>> m_pickups;
	std::vector<std::shared_ptr<Entity>> m_enemies;
	std::vector<std::shared_ptr<BaseGameObject>> m_explosions;
	std::shared_ptr<UIElementBase> m_background;
};
#endif