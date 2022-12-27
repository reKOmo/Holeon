#include "BattleManager.h"
#include "ButtonManager.h"
#include "GameUtils.h"
#include "EntityStats.h"
#include "Helpers.h"
#include "StatsDisplay.h"

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

	statDisplayPlayer = getEntityByName("statsDisplayBottom");
	statDisplayOpponent = getEntityByName("statsDisplayTop");

	auto dis1 = dynamic_cast<StatsDisplay*>(statDisplayOpponent.getComponent<Engine::Components::ScriptComponent>().instance);
	dis1->setTargetEntity(player);

	auto dis2 = dynamic_cast<StatsDisplay*>(statDisplayPlayer.getComponent<Engine::Components::ScriptComponent>().instance);
	dis2->setTargetEntity(opponent);
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
		int result;
		switch (currentDamageStage) {
		case DAMAGE_PHASE_ONE:
			if (atk.currentWeapon.stats.speedMod >= def.currentWeapon.stats.speedMod) {
				text.text = atk.name + " used " + atk.currentWeapon.attacks[playerPickedAtk].name;
				result = dealDamage(atk, playerPickedAtk, def);
			}
			else {
				text.text = def.name + " used " + def.currentWeapon.attacks[playerPickedAtk].name;
				result = dealDamage(def, opponentPickedAtk, atk);
			}

			if (result >= 1 && result <= CRIT_HIT) {
				auto mgr = dynamic_cast<StatsDisplay*>(statDisplayOpponent.getComponent<Engine::Components::ScriptComponent>().instance);
				mgr->updateHp([&]() {
					waitingForAccept = true;
					// need to check if result is a vaild battle stage before assiging
					if (result != INVALID && result > DAMAGE_PHASE_TWO) {
						currentDamageStage = (DAMAGE_TURN_STAGES) result;
						nextDamageStage = DAMAGE_PHASE_TWO;
					}
					else {
						currentDamageStage = DAMAGE_PHASE_TWO;
					}
				});
				currentDamageStage = INVALID;
			}
			break;
		case DAMAGE_PHASE_TWO:
			if (atk.currentWeapon.stats.speedMod < def.currentWeapon.stats.speedMod) {
				text.text = atk.name + " used " + atk.currentWeapon.attacks[playerPickedAtk].name;
				result = dealDamage(atk, playerPickedAtk, def);
			}
			else {
				text.text = def.name + " used " + def.currentWeapon.attacks[playerPickedAtk].name;
				result = dealDamage(def, opponentPickedAtk, atk);
			}

			if (result >= 1 && result <= CRIT_HIT) {
				auto mgr = dynamic_cast<StatsDisplay*>(statDisplayOpponent.getComponent<Engine::Components::ScriptComponent>().instance);
				mgr->updateHp([&]() {
					waitingForAccept = true;
				// need to check if result is a vaild battle stage before assiging
				if (result != INVALID && result > DAMAGE_PHASE_TWO) {
					currentDamageStage = (DAMAGE_TURN_STAGES)result;
					nextDamageStage = DAMAGE_PHASE_TWO;
				}
				else {
					currentDamageStage = DAMAGE_PHASE_TWO;
				}
					});
				currentDamageStage = INVALID;
			}
			break;
		case CRIT_HIT:
			text.text = "Critical hit!";
			waitingForAccept = true;
			currentDamageStage = nextDamageStage;
			nextDamageStage = INVALID;
			break;
		case END_ROUND:
			currentDamageStage = DAMAGE_PHASE_ONE;
			state = PLAYER_TURN;
			waitingForAccept = false;
			break;
		default:
			break;
		}
	}
}

/*
	returs
	0 - nothing
	// damageing ones
	1 - did damage
	3 - crit
	// stat boost
*/
int BattleManager::dealDamage(EntityStats& attacker, int attackIndex, EntityStats& defender) {
	int ret = INVALID;
	//calc damage
	float totalDamage = (attacker.currentWeapon.attacks[attackIndex].baseDamage * (attacker.level / 20.0)) * attacker.currentWeapon.stats.damageMod * (0.9 + Engine::rand() * 0.3);
	float rand = Engine::rand();
	bool isCrit = rand < attacker.currentWeapon.stats.speedMod / 8;

	if (isCrit) {
		totalDamage *= 1.5 + Engine::rand() * 0.5;
		ret = CRIT_HIT;
	}
	//reduce damage
	totalDamage /= defender.baseDefence / 0.8;

	//deal damage
	defender.health -= totalDamage;
	printf("Health: %f\n", defender.health);
	return ret;
}
