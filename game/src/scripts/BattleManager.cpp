#include "BattleManager.h"
#include "ButtonManager.h"
#include "GameUtils.h"
#include "EntityStats.h"
#include "Helpers.h"

void BattleManager::onCreate() {
	player = getEntityByName("player");
	opponent = getEntityByName("opponent");
	battleProgressDialog = getEntityByName("battleDialog");
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
			auto buttonMgr = dynamic_cast<ButtonManager*>(script.instance);
			buttonMgr->onSelect([&](int val) {
				state = DEAL_DAMAGE;
				justSwitched = true;
				playerPickedAtk = val;
			});
			attackUI.enable();
			auto& children = battleProgressDialog.getChildren();
			auto& text = children[0].getComponent<Engine::Components::TextComponent>();
			text.text = "Pick next move";
		}
	}
}

void BattleManager::damageTurn() {
	if (!attackUI.disabled()) {
		attackUI.disable();
	}
	if (justSwitched) {
		justSwitched = false;
	}

	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();


	EntityStats& atk = player.getComponent<EntityStats>();
	EntityStats& def = opponent.getComponent<EntityStats>();

	if (waitingForAccept && IsKeyPressed(KEY_ENTER)) {
		waitingForAccept = false;
	}

	if (!waitingForAccept) {
		switch (damageTurnStage) {
		case 0:
			//deal fisrt damage
			if (atk.currentWeapon.stats.speedMod >= def.currentWeapon.stats.speedMod) {
				std::string base = "Player used ";
				printf("pl");
				text.text = base + atk.currentWeapon.attacks[playerPickedAtk].name;
				dealDamage(atk, playerPickedAtk, def);
			}
			else {
				std::string base = "Opponent used ";
				printf("op");
				text.text = base + def.currentWeapon.attacks[opponentPickedAtk].name;
				dealDamage(def, opponentPickedAtk, atk);
			}
			waitingForAccept = true;
			break;
		case 1:
			//deal fisrt damage
			if (atk.currentWeapon.stats.speedMod < def.currentWeapon.stats.speedMod) {
				std::string base = "Player used ";
				printf("pl");
				text.text = base + atk.currentWeapon.attacks[playerPickedAtk].name;
				dealDamage(atk, playerPickedAtk, def);
			}
			else {
				std::string base = "Opponent used ";
				printf("op");
				text.text = base + def.currentWeapon.attacks[opponentPickedAtk].name;
				dealDamage(def, opponentPickedAtk, atk);
			}
			waitingForAccept = true;
			break;
		case 3:
			damageTurnStage = -1;
			state = PLAYER_TURN;
			waitingForAccept = false;
			break;
		default:
			break;
		}
		damageTurnStage++;
	}
}

void BattleManager::dealDamage(EntityStats& attacker, int attackIndex, EntityStats& defender) {
	//calc damage
	float totalDamage = (attacker.currentWeapon.attacks[attackIndex].baseDamage * (attacker.level / 20.0)) * attacker.currentWeapon.stats.damageMod;
	bool isCrit = Engine::rand() > attacker.currentWeapon.stats.speedMod / 8;
	if (isCrit)
		totalDamage *= 1.0 + Engine::rand();
	//reduce damage
	totalDamage /= defender.baseDefence / 0.8;

	//deal damage
	defender.health -= totalDamage;
}
