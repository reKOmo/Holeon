#pragma once
#include "WeaponStats.h"
#include "AttackStats.h"

struct Weapon {
	AttackStats attacks[4];
	WeaponStats stats;
	std::string name = "Weapon";

	Weapon() {
		for (int i = 0; i < Attack::maxAttacks; i++) {
			attacks[i] = AttackStats();
		}
	}

	Weapon(const Weapon& other) {
		for (int i = 0; i < Attack::maxAttacks; i++) {
			attacks[i] = other.attacks[i];
		}
		stats = other.stats;
		name = other.name;
	}
	
	Weapon& operator=(const Weapon& other) {
		for (int i = 0; i < Attack::maxAttacks; i++) {
			attacks[i] = other.attacks[i];
		}
		stats = other.stats;
		name = other.name;

		return *this;
	}

	Weapon(Weapon&& other) noexcept {
		for (int i = 0; i < Attack::maxAttacks; i++) {
			attacks[i] = other.attacks[i];
		}
		stats = other.stats;
		name = std::move(other.name);
	}

	Weapon& operator=(Weapon&& other) noexcept {
		for (int i = 0; i < Attack::maxAttacks; i++) {
			attacks[i] = other.attacks[i];
		}
		stats = other.stats;
		name = std::move(other.name);

		return *this;
	}
};