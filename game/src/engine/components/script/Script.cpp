#include "Script.h"
#include "Entity.h"

Engine::Entity Engine::Script::getEntityByName(std::string name)
{
    return m_Scene->getEntityByName(name);
}

std::vector<Engine::Entity> Engine::Script::getEntitiesByTag(std::string tag) {
    return m_Scene->getEntitiesByTag(tag);
}

Engine::Entity Engine::Script::Instattiate(std::function<Engine::Entity(Engine::Scene* scene)> func)
{
    return func(m_Scene);
}

Engine::SceneManager& Engine::Script::getSceneManager()
{
    return *m_Scene->m_SceneManager;
}
