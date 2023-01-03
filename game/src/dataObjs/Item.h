#pragma once
#include <string>
#include "AttackStats.h"

struct Item {
	std::string name = "";
	Attack::AttackTypes type = Attack::NULL_ATK;
	float value = 5.0;
};