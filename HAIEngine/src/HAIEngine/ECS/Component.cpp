#include "Component.hpp"
#include "HAIEngine/ECS/GameObject.hpp"

namespace HAIEngine
{
    Transform* Component::GetTransform() const
    {
        return m_fatherGO->GetTransform();
    }

    void Component::SetFatherGO(GameObject* go)
    {
        go->AddComponent(this);
    }
}
