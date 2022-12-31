#pragma once
#include "Engine.h"

class NestTrap : public Engine::Script {
public:
	void onCreate();
	void onTriggerEnter(Engine::Entity col);
private:
	Engine::Entity sceneSwitcher;
};