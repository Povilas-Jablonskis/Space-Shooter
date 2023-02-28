#ifndef soundsMenu_hpp
#define soundsMenu_hpp

#include "Scene.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"

class SoundsMenu final : public Scene
{
public:
	SoundsMenu(SceneStateMachine&, SharedContext&);

	void savePlayerConfig() const;

	void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

	void processInput() override;

	void draw(float) override;

private:
	std::vector<std::shared_ptr<Text>> m_texts;
	SharedContext& m_context;
	SceneStateMachine& m_sceneStateMachine;
};

#endif
