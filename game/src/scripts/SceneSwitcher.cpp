#include "SceneSwitcher.h"
#include "TransitionAnimation.h"

void SceneSwitcher::onCreate() {
}

void SceneSwitcher::switchToScene(int id) {
	auto& animator = m_Owner.getComponent<Engine::Components::AnimationManager>();

	//pause game
	auto toPause = getEntitiesByTag("pauseable");
	for (auto& ent : toPause) {
		printf("pause");
		ent.getComponent<Engine::Components::ScriptComponent>().paused = true;
	}
	getSceneManager().getActveScene().setTimeScale(0.0);

	animator.playAnimation<TransitionAnimation>([&, id]() {
		getSceneManager().loadScene(id);
	});
}