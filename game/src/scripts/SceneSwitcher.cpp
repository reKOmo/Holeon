#include "SceneSwitcher.h"
#include "TransitionAnimation.h"

void SceneSwitcher::onCreate() {
}

void SceneSwitcher::switchToScene(int id) {
	auto& animator = m_Owner.getComponent<Engine::Components::AnimationManager>();

	animator.playAnimation<TransitionAnimation>([&, id]() {
		getSceneManager().loadScene(id);
	});
}