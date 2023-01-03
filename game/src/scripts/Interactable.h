#pragma once
#include "Engine.h"
class Interactable : public Engine::Script {
public:
	virtual void onInteract(std::function<void()> onDone = []() {}) {}
};