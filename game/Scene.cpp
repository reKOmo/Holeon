#include "Scene.h"
#include "Entity.h"

namespace Engine{ 
	Entity Scene::createEntity() {
		Entity e = { m_Registry.create(), &m_Registry };
		return e;
	}
}


