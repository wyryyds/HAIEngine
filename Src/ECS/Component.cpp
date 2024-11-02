#include "Component.hpp"
#include "ECS/GameObject.hpp"

namespace HAIEngine
{
    Component::Component(std::string typeName, GameObject* father)
        : m_typeName(std::move(typeName)), m_fatherGO(father)
    {
        if(father)
            father->AddComponent<Component>(*this);
    }


    json Component::Serialize()
    {
        json resJson;
        resJson["type"] = SerializeHelper::SerializeData(m_typeName);
        resJson["guid"] = SerializeHelper::SerializeData(m_guid);

        return resJson;
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
