#pragma once
#include "Engine.h"

class Interactor : public Engine::Script {
public:
	void onTrigger(Engine::Entity col);
private:
	bool isInteracting = false;
	bool skipNext = false;
};