#pragma once
#include "Engine.h"

struct TempWorldData {
	raylib::Vector2 playerPosition;
	uint32_t triggeredEntity = 0;
	raylib::Vector2 lastSleppPosition;
	bool returnToFireplace = false;
};