#include "Interactor.h"
#include "Interactable.h"

void Interactor::onTrigger(Engine::Entity col) {
	std::string tag = "interactable";
	if (IsKeyPressed(KEY_ENTER) && Engine::findIndex(col.getComponent<Engine::Components::InfoComponent>().tags, tag) != -1 && !isInteracting && !skipNext) {
		isInteracting = true;
		dynamic_cast<Interactable*>(col.getComponent<Engine::Components::ScriptComponent>().instance)->onInteract([&]() {
			isInteracting = false;
		if (IsKeyPressed(KEY_ENTER))
			skipNext = true;
		});
	}
	if (IsKeyPressed(KEY_ENTER) && skipNext) {
		skipNext = false;
	}
}
