#pragma once
#include "Engine.h"
#include <functional>

class StatsDisplay : public Engine::Script {
public:
	void onCreate();
	void onUpdate(float delta);
	void setTargetEntity(Engine::Entity);
	void updateHp(std::function<void()> call);
private:
	Engine::Entity target;
	Engine::Entity fillBar;
	Engine::Entity nameText;
	Engine::Entity levelText;
	void setHpInstant(float currentHp, float maxHp);
	bool animationRunning = false;
};