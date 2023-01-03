#include "Fireplace.h"
#include "DialogManager.h"
#include "SleepAnimation.h"
#include "EntityStats.h"
#include "TempWorldData.h"

Engine::Entity createTransition(Engine::Scene* scene) {
	auto e = scene->createEntity("tra");
	auto& trans = e.addComponent<Engine::Components::TransformComponent>();
	trans.Position = { 0.0, 0.0 };
	trans.Rotation = 0.0;
	Engine::Renderer::Material m = { scene->m_TextureManager->getTexture(14), {0.0, 0.0, 1280.0, 720.0} };
	auto& sp = e.addComponent<Engine::Components::ImageComponent>(m);
	sp.tint = { 0, 0, 0, 255 };

	auto& animMgr = e.addComponent<Engine::Components::AnimationManager>();
	animMgr.useUnscaledTime = true;

	return e;
}

void Fireplace::onCreate() {
	transition = Instattiate(createTransition);
	transition.disable();
}

void Fireplace::onInteract(std::function<void()> onDone) {
	getGlobalStorage().get<TempWorldData>("tempWorldData")->lastSleppPosition = getEntityByName("player").getComponent<Engine::Components::TransformComponent>().Position;
	auto dial = getEntityByName("dialogManager");
	
	dynamic_cast<DialogManager*>(dial.getComponent<Engine::Components::ScriptComponent>().instance)->pickDialog("Sleep?", "Yes", "No", [&, onDone](bool a) {
		if (a) {
			auto toPause = getEntitiesByTag("pauseable");
			for (auto& ent : toPause) {
				ent.getComponent<Engine::Components::ScriptComponent>().paused = true;
			}
			getSceneManager().getActveScene().setTimeScale(0.0);
			transition.enable();
			transition.getComponent<Engine::Components::AnimationManager>().playAnimation<SleepAnimation>([&, onDone]() {
				transition.disable();
				auto toPause = getEntitiesByTag("pauseable");
				for (auto& ent : toPause) {
					ent.getComponent<Engine::Components::ScriptComponent>().paused = false;
				}
				getSceneManager().getActveScene().setTimeScale(1.0);

				//heal player
				auto stats = getGlobalStorage().get<EntityStats>("playerStats");
				stats->health = stats->maxHelath;

				onDone();
			});
		}
		else {
			onDone();
		}
	});
	
}
