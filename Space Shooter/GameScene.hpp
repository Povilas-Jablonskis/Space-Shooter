#ifndef gameScene_hpp
#define gameScene_hpp

#include "Scene.hpp"
#include "SharedContext.hpp"
#include "ObjectCollection.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

class SpriteSheetManager;
class UIManager;
class Object;

class GameScene : public Scene
{
public:
    GameScene(std::shared_ptr<SceneStateMachine>, std::shared_ptr<SpriteSheetManager>, std::shared_ptr<InputManager>, irrklang::ISoundEngine*, ResourceAllocator<Texture>&);

	void loadLevel();

    void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

    void processInput() override;
    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
	std::shared_ptr<Player> m_player{};

	float m_currentTime{ static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) };
	float m_accumulator{};
	int m_maxLevels{};
	int m_currentLevel{};

	SharedContext m_context{};

	S_Drawable m_drawbleSystem;
	S_Collidable m_collisionSystem;

	std::vector<std::shared_ptr<BaseGameObject>> m_meteors;
	std::vector<std::shared_ptr<BaseGameObject>> m_pickups;
	std::vector<std::shared_ptr<Entity>> m_enemies;
	std::vector<std::shared_ptr<BaseGameObject>> m_explosions;
	ObjectCollection m_objects;
	std::shared_ptr<UIElementBase> m_background{};

	ResourceAllocator<Texture>& m_textureAllocator;
	std::shared_ptr<UIManager> m_uiManager{ std::make_shared<UIManager>() };
    std::shared_ptr<InputManager> m_inputManager{};
	std::shared_ptr<SpriteSheetManager> m_spriteSheetManager{};
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{};
	irrklang::ISoundEngine* m_soundEngine{};
};

#endif