#pragma once
#include "WeaponStats.h"
#include "AttackStats.h"

struct Weapon {
	AttackStats attacks[4];
	WeaponStats stats;

	Weapon() {
		for (int i = 0; i < Attack::maxAttacks; i++) {
			attacks[i] = AttackStats();
		}
	}
};