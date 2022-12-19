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
	float damage = 10.0;
	float baseDamage = 10.0;
	Attack::AttackTypes type = Attack::NULL_ATK;
	int element = 0;
	std::string name = "Attack";

	AttackStats() = default;
	AttackStats(const AttackStats&) = default;
	AttackStats(std::string name, float damage = 10.0, Attack::AttackTypes type = Attack::DAMAGE, int elem = 0) : name(name), damage(damage), type(type), element(elem) {}

	operator bool() const {
		return type != Attack::NULL_ATK;
	}
};