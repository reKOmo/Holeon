#pragma once
#include "Engine.h"
#include "EntityStats.h"

enum BATTLE_STATE {
	PLAYER_TURN = 0,
	DEAL_DAMAGE
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
	bool justSwitched = false;
	int damageTurnStage = 0;
	int playerPickedAtk = 0;
	int opponentPickedAtk = 0;
	bool waitingForAccept = false;
	void playerTurn();
	void damageTurn();
	void dealDamage(EntityStats& a, int atk, EntityStats& b);
};