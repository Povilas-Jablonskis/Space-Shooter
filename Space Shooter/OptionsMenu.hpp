#ifndef optionsMenu_hpp
#define optionsMenu_hpp

#include "Scene.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"

class Text;

class OptionsMenu final : public Scene
{
public:
	OptionsMenu(SceneStateMachine&, SharedContext&);

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
