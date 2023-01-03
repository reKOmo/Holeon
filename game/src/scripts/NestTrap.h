#pragma once
#include "Engine.h"

class NestTrap : public Engine::Script {
public:
	void onCreate();
	void onTriggerEnter(Engine::Entity col);
	void onTriggerExit(Engine::Entity col);
	bool isTriggered = false;
private:
	Engine::Entity sceneSwitcher;
};