#ifndef gameScene_hpp
#define gameScene_hpp

#include "Scene.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

class SpriteSheetManager;
class Player;
class Entity;
class UIElementBase;
class BaseGameObject;
class UIManager;

class GameScene : public Scene
{
public:
    GameScene(std::shared_ptr<SceneStateMachine>, std::shared_ptr<SpriteSheetManager>, std::shared_ptr<InputManager>, irrklang::ISoundEngine*);

	void loadLevel();

    void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

    void processInput() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
	std::shared_ptr<Player> m_player{ nullptr };

	float m_currentTime{ static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) };
	float m_accumulator{ 0.0f };
	int m_maxLevels{ 0 };
	int m_currentLevel{ 0 };

	std::vector<std::shared_ptr<BaseGameObject>> m_meteors;
	std::vector<std::shared_ptr<BaseGameObject>> m_pickups;
	std::vector<std::shared_ptr<Entity>> m_enemies;
	std::vector<std::shared_ptr<BaseGameObject>> m_explosions;
	std::shared_ptr<UIElementBase> m_background;

	std::shared_ptr<UIManager> m_uiManager{ std::make_shared<UIManager>() };
    std::shared_ptr<InputManager> m_inputManager{ nullptr };
	std::shared_ptr<SpriteSheetManager> m_spriteSheetManager{ nullptr };
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{ nullptr };
	irrklang::ISoundEngine* m_soundEngine{ nullptr };
};

#endif /* SceneSplashScreen_hpp */