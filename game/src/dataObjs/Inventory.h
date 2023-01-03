#pragma once
#include "Weapon.h"
#include "Item.h"
#include <vector>

struct Inventory {
	std::vector<Weapon> weapons = {
		Weapon()
	};
	std::vector<Item> items = {};
};