#pragma once
#include "Weapon.h"

struct EntityStats {
	float health = 10.0;
	float maxHelath = 10.0;
	float defence = 1.0;
	float baseDefence = 1.0;
	int level = 10;
	Weapon currentWeapon;
};