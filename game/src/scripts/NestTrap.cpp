#include "NestTrap.h"
#include "SceneSwitcher.h"

void NestTrap::onCreate() {
	sceneSwitcher = getEntityByName("battle-transition");
}

void NestTrap::onTriggerEnter(Engine::Entity col) {
	auto& info = col.getComponent<Engine::Components::InfoComponent>();
	if (info.name == "player" && Engine::rand() < 0.6) {
		auto& sprite = m_Owner.getParent().getComponent<Engine::Components::SpriteComponent>();
		sprite.imageIndex = 1;
		auto& script = sceneSwitcher.getComponent<Engine::Components::ScriptComponent>();
		static_cast<SceneSwitcher*>(script.instance)->switchToScene(1);
	}
}
