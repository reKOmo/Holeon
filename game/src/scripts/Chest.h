#pragma once
#include "Interactable.h"

class Chest : public Interactable {
public:
	void onInteract(std::function<void()> onDone = []() {});
private:
	bool wasOpened = false;
};