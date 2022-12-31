#pragma once
#include "Engine.h"

class SceneSwitcher : public Engine::Script {
public:
	void onCreate();
	void switchToScene(int id);
private:
	Engine::Entity transition;
};