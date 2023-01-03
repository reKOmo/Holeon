#pragma once
#include "Weapon.h"
#include "Item.h"
#include <vector>

struct Inventory {
	std::vector<Weapon> weapons = {};
	std::vector<Item> items = {
		Item("Heal potion", Attack::HEAL, 5.0),
		Item("Energy bar", Attack::SPD_BOOST, 0.3),
	};
};