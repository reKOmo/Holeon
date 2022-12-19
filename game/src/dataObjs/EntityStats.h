#pragma once
#include "Weapon.h"

struct EntityStats {
	float health = 10.0;
	float maxHelath = 10.0;
	float shield = 10.0;
	float maxShield = 10.0;
	int level = 10;
	Weapon currentWeapon;
};