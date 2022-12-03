#include "Script.h"
#include "Entity.h"

Engine::Entity Engine::Script::getEntityByName(std::string name)
{
    return m_Scene->getEntityByName(name);
}
