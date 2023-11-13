#include "Scene.hpp"

namespace HAIEngine
{
    Scene::Scene(const std::string& filepath)
        : m_sceneName("Scene"), m_serializeFile(filepath)
    {
        DeSerialize(m_serializeFile.GetJsonData());
    }

    Scene::Scene(std::string sceneName, const std::string& filepath)
        : m_sceneName(sceneName), m_serializeFile(filepath)
    {
        DeSerialize(m_serializeFile.GetJsonData());
    }

    void HAIEngine::Scene::Save()
    {
        Serialize(m_sceneName);
    }

    std::shared_ptr<GameObject> HAIEngine::Scene::Find(const std::string& name)
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

    json HAIEngine::Scene::Serialize(const std::string& name)
    {
        json sceneData;
        sceneData["sceneName"] = m_sceneName;
        sceneData["guid"] = m_guid;
        json gosData = json::array();
        for (auto& go : m_gameObjects)
        {
            gosData.push_back(go->Serialize(go->m_name));
        }
        sceneData["gameObjects"] = gosData;
        return sceneData;
    }

    void HAIEngine::Scene::DeSerialize(const json& jsondata)
    {
        m_sceneName = jsondata["sceneName"];
        m_guid = jsondata["guid"];

        json gameObjects = jsondata["gameObjects"];
        for (int i = 0; i < gameObjects.size(); ++i)
        {
            auto go = std::make_shared<GameObject>();
            go->DeSerialize(gameObjects[i]);
            AddGameObject(go);
        }
    }
}
