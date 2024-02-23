#include "Component.hpp"
#include "ECS/GameObject.hpp"

namespace HAIEngine
{
    Component::Component(std::string typeName, GameObject* father)
        : m_typeName(typeName)
    {
        father->AddComponent(this);
    }

    Transform& Component::GetTransform() const
    {
        return m_fatherGO->GetTransform();
    }

    void Component::SetFatherGO(GameObject* go)
    {
        m_fatherGO = go;
    }
}
