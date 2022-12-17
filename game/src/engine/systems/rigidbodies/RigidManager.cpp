#include "RigidManager.h"
#include "Components.h"

void Engine::Systemns::updateRigidbodies(entt::registry* scene, float delta) {
	scene->view<Engine::Components::RigidbodyComponent, Engine::Components::ColiderComponent>().each([](entt::entity ent, Engine::Components::RigidbodyComponent rb, Engine::Components::ColiderComponent c) {
		
	});
}
