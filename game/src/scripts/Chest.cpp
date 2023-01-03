#include "Chest.h"
#include "DialogManager.h"
#include "Inventory.h"

void Chest::onInteract(std::function<void()> onDone) {
	if (!wasOpened) {
		auto dial = getEntityByName("dialogManager");

		wasOpened = true;
		int index;
		if (Engine::rand() > 0.5) {
			index = 0;
		}
		else {
			index = 1;
		}
		getGlobalStorage().get<Inventory>("inventory")->items[index].amout++;

		std::string t = "Found " + getGlobalStorage().get<Inventory>("inventory")->items[index].name + "!";

		dynamic_cast<DialogManager*>(dial.getComponent<Engine::Components::ScriptComponent>().instance)->showDialog(t, [onDone]() {
			onDone();
		});

	} else {
		onDone();
	}
}
