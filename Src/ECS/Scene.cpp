#include "Scene.hpp"
#include "GameObject.hpp"

namespace HAIEngine
{
    Scene::Scene(const std::string& filepath)
        : m_sceneName("Scene"), m_serializeFile(filepath)
    {
    }

    Scene::Scene(std::string sceneName, const std::string& filepath)
        : m_sceneName(sceneName), m_serializeFile(filepath)
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

    std::shared_ptr<GameObject> Scene::Find(const std::string& name)
    {
        for (auto& go : m_gameObjects)
        {
            if (go->m_name == name)
            {
                return go;
            }
        }
        HE_CORE_ERROR("No sunch name GameObject!");
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

    void Scene::DeSerialize(const json& jsondata)
    {
        m_sceneName = SerializeHelper::DeSerializeData<std::string>(jsondata["sceneName"]);
        m_guid = SerializeHelper::DeSerializeData<size_t>(jsondata["guid"]);

        json gameObjects = jsondata["gameObjects"];
        for (int i = 0; i < gameObjects.size(); ++i)
        {
            auto go = std::make_shared<GameObject>();
            go->DeSerialize(gameObjects[i]);
            AddGameObject(go);
        }
    }
}
