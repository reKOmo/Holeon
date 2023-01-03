#pragma once
#include "Engine.h"

struct TempWorldData {
	raylib::Vector2 playerPosition;
	uint32_t triggeredEntity = 0;
	raylib::Vector2 lastSleppPosition = { 608.0, 288.0 };
	bool returnToFireplace = false;
};