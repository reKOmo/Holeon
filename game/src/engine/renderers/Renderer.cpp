#include "Renderer.h"

void Engine::Renderer::getAbsoluteTransform(Engine::Entity currentEnt, Engine::Components::TransformComponent& trans) {
	auto& parent = currentEnt.getParent();
	if (parent) {
		auto& parentTransform = parent.getComponent<Engine::Components::TransformComponent>();
		//rotate
		float s = sin(DEG2RAD * parentTransform.Rotation);
		float c = cos(DEG2RAD * parentTransform.Rotation);

		trans.Position = { trans.Position.x * c - trans.Position.y * s, trans.Position.x * s + trans.Position.y * c };

		trans.Position += parentTransform.Position;
		trans.Rotation += parentTransform.Rotation;

		getAbsoluteTransform(parent, trans);
	}
}

Engine::Components::TransformComponent Engine::Renderer::getAbsoluteTransform(Engine::Entity ent) {
	Engine::Components::TransformComponent t(ent.getComponent<Engine::Components::TransformComponent>());

	getAbsoluteTransform(ent, t);

	return t;
}