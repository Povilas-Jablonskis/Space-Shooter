#ifndef gameScene_hpp
#define gameScene_hpp

#include "Scene.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"
#include "ObjectCollection.hpp"
#include "SpriteSheet.hpp"
#include "Configs.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

class GameScene final : public Scene
{
public:
	GameScene(SceneStateMachine& sceneStateMachine, SharedContext& context, const int characterSelectionIndex);

	void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

	void processInput() override;
	void draw(float dt) override;

private:
	void loadLevel();

	float m_currentTime{static_cast<float>(glutGet(GLUT_ELAPSED_TIME))};
	float m_accumulator{};
	int m_maxLevels{};
	int m_currentLevel{};
	int m_characterSelectionIndex{};

	SpriteSheet m_spriteSheet{Configs::BACKGROUND_PATH};
	ObjectCollection m_objects;
	SharedContext& m_context;
	SceneStateMachine& m_sceneStateMachine;
};

#endif
