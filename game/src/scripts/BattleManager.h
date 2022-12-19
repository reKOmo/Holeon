#pragma once
#include "Engine.h"

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
	void playerTurn();
	void damageTurn();
};