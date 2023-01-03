#include "BattleManager.h"
#include "ButtonManager.h"
#include "GameUtils.h"
#include "EntityStats.h"
#include "Helpers.h"
#include "StatsDisplay.h"
#include "TempWorldData.h"
#include "BattlePlayerUI.h"

#include <format>

void BattleManager::onCreate() {
	playerUIManager = getEntityByName("uiManager");

	playerStats = getGlobalStorage().get<EntityStats>("playerStats");
	opponentStats = getGlobalStorage().get<EntityStats>("opponentStats");

	battleProgressDialog = getEntityByName("battleDialog");

	statDisplayPlayer = getEntityByName("statsDisplayBottom");
	statDisplayOpponent = getEntityByName("statsDisplayTop");
	auto dis1 = dynamic_cast<StatsDisplay*>(statDisplayOpponent.getComponent<Engine::Components::ScriptComponent>().instance);
	dis1->setTarget(opponentStats);
	auto dis2 = dynamic_cast<StatsDisplay*>(statDisplayPlayer.getComponent<Engine::Components::ScriptComponent>().instance);
	dis2->setTarget(playerStats);
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
	auto script = playerUIManager.getComponent<Engine::Components::ScriptComponent>().instance;
	if (script && !showingPlayerUI) {
		auto mgr = dynamic_cast<BattleUIPlayer*>(script);
		mgr->performAction([&](auto& a) {
			auto& [actionType, value] = a;
			playerPickedAtk = value;
			playerPickedAction = actionType;
			battleProgressDialog.getComponent<Engine::Components::TransformComponent>().Position.y = 300.0;
			showingPlayerUI = false;
			state = DEAL_DAMAGE;
		});
		showingPlayerUI = true;
	}
}

void BattleManager::damageTurn() {
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
		case GRANT_EXP:
			grantExpPhase();
			break;
		case LEVEL_UP:
			levelUpPhase();
			break;
		case END_BATTLE:
			endBattlePhase();
			break;
		default:
			break;
		}
	}
}

void BattleManager::setupPhase() {
	if (playerStats->currentWeapon.stats.speedMod >= opponentStats->currentWeapon.stats.speedMod) {
		firstAttacker = playerStats;
		secondAttacker = opponentStats;
	}
	else {
		firstAttacker = opponentStats;
		secondAttacker = playerStats;
	}
	currentDamageStage = DAMAGE_PHASE_ONE;
}

void BattleManager::damagePhaseOne() {
	if (firstAttacker == playerStats) {
		damagePhase(firstAttacker, secondAttacker, playerPickedAtk, statDisplayOpponent, DAMAGE_PHASE_TWO);
	}
	else {
		damagePhase(firstAttacker, secondAttacker, opponentPickedAtk, statDisplayPlayer, DAMAGE_PHASE_TWO);
	}
}

void BattleManager::damagePhaseTwo() {
	if (secondAttacker == playerStats) {
		damagePhase(secondAttacker, firstAttacker, playerPickedAtk, statDisplayOpponent, END_ROUND);
	}
	else {
		damagePhase(secondAttacker, firstAttacker, opponentPickedAtk, statDisplayPlayer, END_ROUND);
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
	text.text = playerStats->name + "'s condition fatal...\nFleeing";
	currentDamageStage = END_BATTLE;
	waitingForAccept = true;
}

void BattleManager::opponentLostPhase() {
	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	text.text = opponentStats->name + " has been defeated!";
	currentDamageStage = GRANT_EXP;
	waitingForAccept = true;
}

void BattleManager::grantExpPhase() {
	const int baseXP = 64;
	int gainedXp = (opponentStats->level * baseXP) / 7;
	playerStats->xp += gainedXp;

	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	std::string t = "Gained ";
	text.text = t + std::to_string(gainedXp) + "XP!";
	currentDamageStage = LEVEL_UP;
	waitingForAccept = true;
}

void BattleManager::levelUpPhase() {
	int requiredXp = std::pow(playerStats->level, 3) - std::pow(playerStats->level - 1, 3);

	bool grew = false;

	do {
		if (playerStats->xp >= requiredXp) {
			playerStats->level++;
			playerStats->xp -= requiredXp;
			grew = true;
			requiredXp = std::pow(playerStats->level, 3) - std::pow(playerStats->level - 1, 3);
		}
	} while (playerStats->xp >= requiredXp);

	if (grew) {
		auto& children = battleProgressDialog.getChildren();
		auto& text = children[0].getComponent<Engine::Components::TextComponent>();
		std::string t = "Grew to level ";
		text.text = t + std::to_string(playerStats->level) + "!";
		waitingForAccept = true;
	}
	currentDamageStage = END_BATTLE;
}

void BattleManager::endBattlePhase() {
	if (playerStats->health <= 0) {
		getGlobalStorage().get<TempWorldData>("tempWorldData")->returnToFireplace = true;
	}
	getSceneManager().loadScene(0);
}

void BattleManager::checkHealth() {
	if (playerStats->health <= 0) {
		// player lost
		currentDamageStage = PLAYER_LOST;
	}
	else if (opponentStats->health <= 0) {
		//opponent lost
		currentDamageStage = OPPONENT_LOST;
	}
}

void BattleManager::performPlayerAction() {
	//!!!!!!!!!!!!!!!!!!!!!!!!!!
}

/*
	returs
	0 - attack missed / to be added
	1 - dealt damage
	2 - crit
*/
int BattleManager::dealDamage(EntityStats* attacker, int attackIndex, EntityStats* defender) {
	int ret = 1;
	//calc damage
	float totalDamage = (attacker->currentWeapon.attacks[attackIndex].baseDamage * (attacker->level / 20.0)) * attacker->currentWeapon.stats.damageMod * (0.9 + Engine::rand() * 0.3);
	float rand = Engine::rand();
	bool isCrit = rand < attacker->currentWeapon.stats.speedMod / 8;

	if (isCrit) {
		totalDamage *= 1.5 + Engine::rand() * 0.5;
		ret = 2;
	}
	//reduce damage
	totalDamage /= defender->baseDefence / 0.8;

	//deal damage
	defender->health -= totalDamage;
	printf("Health: %f\n", defender->health);
	return ret;
}

void BattleManager::damagePhase(EntityStats* atk, EntityStats* def, int pickedAttack, Engine::Entity& affectedStaDisplay, DAMAGE_TURN_STAGES nextTurn) {
	auto& children = battleProgressDialog.getChildren();
	auto& text = children[0].getComponent<Engine::Components::TextComponent>();
	int result;

	text.text = atk->name + " used " + atk->currentWeapon.attacks[pickedAttack].name;
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
