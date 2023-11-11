#include "Camera.hpp"
#include "hepch.hpp"
#include "Core/Log.hpp"
#include "HAIEngine/ECS/GameObject.hpp"
#include "HAIEngine/ECS/Components/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace HAIEngine
{
    HAIEngine::Camera::Camera(CameraType cameraType, float aspectRatio, float fov, float znear, float zfar)
        : m_cameraType(cameraType), m_aspect(aspectRatio)
    {
        m_perspective.fov = fov;
        m_perspective.znear = znear;
        m_perspective.zfar = zfar;
    }

    void HAIEngine::Camera::Update()
    {
    }

    json HAIEngine::Camera::Serialize(const std::string& name)
    {
        return json();
    }

    void HAIEngine::Camera::DeSerialize(const json& jsondata)
    {
    }

    void HAIEngine::Camera::UpdateView()
    {
        if (m_cameraType == CameraType::PERSPECTIVE)
        {
            m_position = m_fatherGO->m_transform->m_position;

            m_view = glm::lookAt(m_position, m_position + m_Front, m_Up);
        }
        else if (m_cameraType == CameraType::ORTHO)
        {
            // TODO
        }
    }
}
