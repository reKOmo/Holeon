#pragma once
#include "Engine.h"
#include "EntityStats.h"

enum BATTLE_STATE {
	PLAYER_TURN = 0,
	DEAL_DAMAGE
};

enum DAMAGE_TURN_STAGES {
	INVALID = 0,
	SETUP,
	DAMAGE_PHASE_ONE,
	DAMAGE_PHASE_TWO,
	CRIT_HIT,
	CHAECK_HEALTH,
	PLAYER_LOST,
	OPPONENT_LOST,
	END_ROUND
};

class BattleManager : public Engine::Script {
public:
	void onCreate();
	void onUpdate(float);
private:
	BATTLE_STATE state = PLAYER_TURN;
	Engine::Entity attackUI;
	Engine::Entity player;
	Engine::Entity opponent;
	Engine::Entity battleProgressDialog;
	Engine::Entity statDisplayPlayer;
	Engine::Entity statDisplayOpponent;
	Engine::Entity firstAttacker;
	Engine::Entity secondAttacker;
	bool justSwitched = false;
	DAMAGE_TURN_STAGES currentDamageStage = SETUP;
	DAMAGE_TURN_STAGES nextDamageStage = INVALID;
	int playerPickedAtk = 0;
	int opponentPickedAtk = 0;
	bool waitingForAccept = false;
	void playerTurn();
	void damageTurn();
	int dealDamage(EntityStats& a, int atk, EntityStats& b);
	void damagePhase(EntityStats& atk, EntityStats& def, int pickedAttack, Engine::Entity& affectedStaDisplay, DAMAGE_TURN_STAGES nextTurn);
	void setupPhase();
	void damagePhaseOne();
	void damagePhaseTwo();
	void critHitPhase();
	void playerLostPhase();
	void opponentLostPhase();
	void checkHealth();
};