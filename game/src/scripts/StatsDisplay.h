#pragma once
#include "Engine.h"
#include "EntityStats.h"
#include <functional>

class StatsDisplay : public Engine::Script {
public:
	void onCreate();
	void onUpdate(float delta);
	void setTarget(EntityStats* stats);
	void updateHp(std::function<void()> call);
private:
	EntityStats* target;
	Engine::Entity fillBar;
	Engine::Entity nameText;
	Engine::Entity levelText;
	void setHpInstant(float currentHp, float maxHp);
	bool animationRunning = false;
};