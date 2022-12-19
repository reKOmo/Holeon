#include "BattleManager.h"
#include "ButtonManager.h"
#include "GameUtils.h"
#include "EntityStats.h"

void BattleManager::onCreate() {
	player = getEntityByName("player");
	opponent = getEntityByName("opponent");
	attackUI = Instattiate([&](Engine::Scene* scene) {
		Engine::Entity buttons[Attack::maxAttacks];

		auto battleUI = scene->createEntity("battleUI");
		auto& uiTrans = battleUI.addComponent<Engine::Components::TransformComponent>();
		

		battleUI.addComponent<Engine::Components::ScriptComponent>().bind<ButtonManager>();
		auto& uiMgrData = battleUI.addComponent<Engine::Components::ButtonManagerData>();

		auto& weapon = player.getComponent<EntityStats>().currentWeapon;
		for (int i = 0; i < Attack::maxAttacks; i++) {
			if (weapon.attacks[i]) {
				auto button = createDialogButton(weapon.attacks[i].name, "button", i, scene);
				if (i > 0) {
					button.getComponent<Engine::Components::ButtonComponent>().top = buttons[i - 1];
					buttons[i - 1].getComponent<Engine::Components::ButtonComponent>().bottom = button;

					auto& buttonTrans = button.getComponent<Engine::Components::TransformComponent>();
					buttonTrans.Position = { 0.0, (float)i * 60.0f };
				}
				button.setParent(battleUI);
				if (i == 0) {
					uiMgrData.currentButton = button;
				}
				buttons[i] = button;
			}
		}

		return battleUI;
	});
	attackUI.disable();
}

void BattleManager::onUpdate(float delta) {
	switch (state) {
	case PLAYER_TURN:
		playerTurn();
		break;
	case DEAL_DAMAGE:
		damageTurn();
		break;
	default:
		break;
	}
}

void BattleManager::playerTurn() {
	if (attackUI.disabled()) {
		auto& script = attackUI.getComponent<Engine::Components::ScriptComponent>();
		if (script) {
			attackUI.enable();
			printf("wwwwwwwwww\n");
			auto buttonMgr = dynamic_cast<ButtonManager*>(script.instance);
			buttonMgr->onSelect([&](int val) {
				printf("Picked %d\n", val);
				state = DEAL_DAMAGE;
			});
		}
	}
}

void BattleManager::damageTurn() {
	printf("Deal damage");
	state = PLAYER_TURN;
}
