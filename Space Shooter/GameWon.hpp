#ifndef gameWon_hpp
#define gameWon_hpp

#include "Scene.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"
#include "Text.hpp"

class GameWon final : public Scene
{
public:
	GameWon(SceneStateMachine& sceneStateMachine, SharedContext& context);

	void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

	void processInput() override;

	void draw(float dt) override;

private:
	std::vector<std::shared_ptr<Text>> m_texts;
	SharedContext& m_context;
	SceneStateMachine& m_sceneStateMachine;
};

#endif
