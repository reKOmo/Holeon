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
	dis1->setTargetEntity(opponent);

	auto dis2 = dynamic_cast<StatsDisplay*>(statDisplayPlayer.getComponent<Engine::Components::ScriptComponent>().instance);
	dis2->setTargetEntity(player);
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

	if (waitingForAccept && IsKeyPressed(KEY_ENTER)) {
		waitingForAccept = false;
	}

	if (!waitingForAccept) {
		switch (currentDamageStage) {
		case SETUP:
			setupPhase();
			break;
		case DAMAGE_PHASE_ONE:
			damagePhaseOne();
			break;
		case DAMAGE_PHASE_TWO:
			damagePhaseTwo();
			break;
		case CRIT_HIT:
			critHitPhase();
			break;
		case END_ROUND:
			currentDamageStage = SETUP;
			state = PLAYER_TURN;
			waitingForAccept = false;
			break;
		case PLAYER_LOST:
			playerLostPhase();
			break;
		case OPPONENT_LOST:
			opponentLostPhase();
			break;
		default:
			break;
		}
	}
}

void BattleManager::setupPhase() {
	EntityStats& pl = player.getComponent<EntityStats>();
	EntityStats& op = opponent.getComponent<EntityStats>();
	if (pl.currentWeapon.stats.speedMod >= op.currentWeapon.stats.speedMod) {
		firstAttacker = player;
		secondAttacker = opponent;
	}
	else {
		firstAttacker = opponent;
		secondAttacker = player;
	}
	currentDamageStage = DAMAGE_PHASE_ONE;
}

void BattleManager::damagePhaseOne() {
	EntityStats& atk = firstAttacker.getComponent<EntityStats>();
	EntityStats& def = secondAttacker.getComponent<EntityStats>();
	if (firstAttacker == player) {
		damagePhase(atk, def, playerPickedAtk, statDisplayOpponent, DAMAGE_PHASE_TWO);
	}
	else {
		damagePhase(atk, def, opponentPickedAtk, statDisplayPlayer, DAMAGE_PHASE_TWO);
	}
}

void BattleManager::damagePhaseTwo() {
	EntityStats& atk2 = secondAttacker.getComponent<EntityStats>();
	EntityStats& def2 = firstAttacker.getComponent<EntityStats>();
	if (secondAttacker == player) {
		damagePhase(atk2, def2, playerPickedAtk, statDisplayOpponent, END_ROUND);
	}
	else {
		damagePhase(atk2, def2, opponentPickedAtk, statDisplayPlayer, END_ROUND);
	}
}

void BattleManager::critHitPhase() {
	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	text.text = "Critical hit!";
	waitingForAccept = true;
	currentDamageStage = nextDamageStage;
	nextDamageStage = INVALID;
	checkHealth();
}

void BattleManager::playerLostPhase() {
	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	text.text = player.getComponent<EntityStats>().name + "'s contition fatal...\nFleeing";
	waitingForAccept = true;
}

void BattleManager::opponentLostPhase() {
	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	text.text = opponent.getComponent<EntityStats>().name + " has been defeated!";
	waitingForAccept = true;
}

void BattleManager::checkHealth() {
	EntityStats& pl = player.getComponent<EntityStats>();
	EntityStats& op = opponent.getComponent<EntityStats>();

	if (pl.health <= 0) {
		// player lost
		currentDamageStage = PLAYER_LOST;
	}
	else if (op.health <= 0) {
		//opponent lost
		currentDamageStage = OPPONENT_LOST;
	}
}

/*
	returs
	0 - attack missed / to be added
	1 - dealt damage
	2 - crit
*/
int BattleManager::dealDamage(EntityStats& attacker, int attackIndex, EntityStats& defender) {
	int ret = 1;
	//calc damage
	float totalDamage = (attacker.currentWeapon.attacks[attackIndex].baseDamage * (attacker.level / 20.0)) * attacker.currentWeapon.stats.damageMod * (0.9 + Engine::rand() * 0.3);
	float rand = Engine::rand();
	bool isCrit = rand < attacker.currentWeapon.stats.speedMod / 8;

	if (isCrit) {
		totalDamage *= 1.5 + Engine::rand() * 0.5;
		ret = 2;
	}
	//reduce damage
	totalDamage /= defender.baseDefence / 0.8;

	//deal damage
	defender.health -= totalDamage;
	printf("Health: %f\n", defender.health);
	return ret;
}

void BattleManager::damagePhase(EntityStats& atk, EntityStats& def, int pickedAttack, Engine::Entity& affectedStaDisplay, DAMAGE_TURN_STAGES nextTurn) {
	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	int result;

	text.text = atk.name + " used " + atk.currentWeapon.attacks[pickedAttack].name;
	result = dealDamage(atk, playerPickedAtk, def);

	if (result >= 1 && result <= 10) {
		// attack some kid of damage
		auto mgr = dynamic_cast<StatsDisplay*>(affectedStaDisplay.getComponent<Engine::Components::ScriptComponent>().instance);
		mgr->updateHp([&, nextTurn, result]() {
			waitingForAccept = true;
			switch (result) {
			case 2:
				currentDamageStage = CRIT_HIT;
				nextDamageStage = nextTurn;
				break;
			default:
				currentDamageStage = nextTurn;
				checkHealth();
				break;
			}
		});
		currentDamageStage = INVALID;
	}
	else {
		waitingForAccept = true;
		currentDamageStage = nextTurn;
	}
}
