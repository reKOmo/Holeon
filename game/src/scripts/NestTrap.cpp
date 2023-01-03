#include "NestTrap.h"
#include "SceneSwitcher.h"
#include "TempWorldData.h"

void NestTrap::onCreate() {
	if (getGlobalStorage().has("tempWorldData")) {
		if (getGlobalStorage().get<TempWorldData>("tempWorldData")->triggeredEntity == m_Owner.getComponent<Engine::Components::InfoComponent>().id()) {
			isTriggered = true;
		}
	}
	sceneSwitcher = getEntityByName("battle-transition");
}

void NestTrap::onTriggerEnter(Engine::Entity col) {
	auto& info = col.getComponent<Engine::Components::InfoComponent>();
	if (info.name == "player" && Engine::rand() < 0.6 && !isTriggered) {
		auto& sprite = m_Owner.getParent().getComponent<Engine::Components::SpriteComponent>();
		sprite.imageIndex = 1;
		getGlobalStorage().get<TempWorldData>("tempWorldData")->triggeredEntity = m_Owner.getComponent<Engine::Components::InfoComponent>().id();
		auto& script = sceneSwitcher.getComponent<Engine::Components::ScriptComponent>();
		static_cast<SceneSwitcher*>(script.instance)->switchToScene(1);
	}
}

void NestTrap::onTriggerExit(Engine::Entity col) {
	isTriggered = false;
}
