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
	int state = PLAYER_TURN;
};