#pragma once
#include "Engine.h"
#include "EntityStats.h"

namespace Player {
	enum Action {
		DEAL_DAMAGE = 0,
		SWITCH_WEAPON,
		USE_ITEM
	};
}


class BattleUIPlayer : public Engine::Script {
public:
	void onCreate();
	void onUpdate(float delta);
	void performAction(std::function<void(std::tuple<Player::Action, int>)> onPicked);
private:
	bool isShown = false;
	bool isSetup = false;
	int currentMenuId = 0;
	std::function<void(std::tuple<Player::Action, int>)> onPicked;
	Engine::Entity currentMenu;
	Engine::Entity attackUI;
	Engine::Entity weaponsUI;
	Engine::Entity itemsUI;
	EntityStats* playerStats = nullptr;
	void onSelect(int val);
	void createWeaponButtons();
};