#include "Scene.hpp"
#include "GameObject.hpp"

namespace HAIEngine
{
    Scene::Scene(std::string_view filepath)
        : m_sceneName("Scene"), m_serializeFile(filepath)
    {
        
    }

    Scene::Scene(std::string sceneName, std::string_view filepath)
        : m_sceneName(std::move(sceneName)), m_serializeFile(filepath)
    {
    }

    void Scene::Load()
    {
        m_serializeFile.Load();
        DeSerialize(m_serializeFile.GetJsonData());
    }

    void Scene::Save()
    {
        m_serializeFile.SetJsonData(Serialize());
        m_serializeFile.Save();
    }

    std::shared_ptr<GameObject> Scene::Find(std::string_view name)
    {
        for (auto& go : m_gameObjects)
        {
            if (go->m_name == name)
            {
                return go;
            }
        }
        HE_CORE_ERROR("No such name GameObject!");
        return nullptr;
    }

    json Scene::Serialize()
    {
        json sceneData;
        sceneData["sceneName"] = SerializeHelper::SerializeData(m_sceneName);
        sceneData["guid"] = SerializeHelper::SerializeData(m_guid);

        json gosData = json::array();
        for (auto& go : m_gameObjects)
        {
            gosData.push_back(go->Serialize());
        }
        sceneData["gameObjects"] = gosData;

        return sceneData;
    }

    void Scene::DeSerialize(const json& jsonData)
    {
        m_sceneName = JsonSerializeHelper::DeSerializeData<std::string>(jsonData["sceneName"]);
        m_guid = JsonSerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

        json gameObjects = jsonData["gameObjects"];
        for (const auto& gameObject : gameObjects)
        {
            auto go = std::make_shared<GameObject>();
            go->DeSerialize(gameObject);
            AddGameObject(go);
        }
    }
}
