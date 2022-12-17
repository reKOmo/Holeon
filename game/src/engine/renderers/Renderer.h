#pragma once
#include "Entity.h"
#include "Components.h"

namespace Engine::Renderer {
	void getAbsoluteTransform(Engine::Entity currentEnt, Engine::Components::TransformComponent& trans);
	Engine::Components::TransformComponent getAbsoluteTransform(Engine::Entity ent);
}