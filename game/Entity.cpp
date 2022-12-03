#include "Entity.h"
#include "Components.h"

namespace Engine {
	Entity::Entity(entt::entity id, entt::registry* reg) : m_EnttID(id), m_Scene(reg) {}
}