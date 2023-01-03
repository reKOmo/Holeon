#pragma once
#include "Interactable.h"

class Fireplace : public Interactable {
public:
	void onCreate();
	void onInteract(std::function<void()> onDone = []() {});
private:
	Engine::Entity transition;
};