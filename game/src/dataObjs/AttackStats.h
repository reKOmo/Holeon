#pragma once
#include <string>

namespace Attack {
	constexpr auto maxAttacks = 4;

	enum AttackTypes {
		DAMAGE = 0,
		DEF_BOOST,
		ATK_BOOST,
		SPD_BOOST,
		NULL_ATK
	};
}

struct AttackStats {
	float baseDamage = 10.0;
	Attack::AttackTypes type = Attack::NULL_ATK;
	std::string name = "Attack";

	AttackStats() = default;
	AttackStats(const AttackStats&) = default;
	AttackStats(std::string name, float damage = 10.0, Attack::AttackTypes type = Attack::DAMAGE, int elem = 0) : name(name), baseDamage(damage), type(type) {}

	operator bool() const {
		return type != Attack::NULL_ATK;
	}
};