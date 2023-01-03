#pragma once
#include <string>
#include "AttackStats.h"

struct Item {
	std::string name = "";
	Attack::AttackTypes type = Attack::NULL_ATK;
	float value = 5.0;
	int amout = 0;

	Item(std::string s, Attack::AttackTypes t, float val) : name(s), type(t), value(val) {}
};