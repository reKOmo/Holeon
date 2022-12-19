#include "Script.h"
#include "Entity.h"

Engine::Entity Engine::Script::getEntityByName(std::string name)
{
    return m_Scene->getEntityByName(name);
}

Engine::Entity Engine::Script::Instattiate(std::function<Engine::Entity(Engine::Scene* scene)> func)
{
    return func(m_Scene);
}
