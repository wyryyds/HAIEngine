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

    void HAIEngine::Camera::Update(TimeStep ts)
    {
        UpdateView();
        UpdateProjection();
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
            m_Front = m_fatherGO->m_transform->GetFront();
            m_Up = m_fatherGO->m_transform->GetUp();

            m_view = glm::lookAt(m_position, m_position + m_Front, m_Up);
        }
        else if (m_cameraType == CameraType::ORTHO)
        {
            // TODO
        }
    }

    void HAIEngine::Camera::UpdateProjection()
    {
        if (m_cameraType == CameraType::PERSPECTIVE)
        {
            m_projection = glm::perspective(glm::radians(m_perspective.fov), m_aspect,
                m_perspective.znear, m_perspective.zfar);
        }
        else if (m_cameraType == CameraType::ORTHO)
        {
            m_projection = glm::ortho(m_ortho.left, m_ortho.right, m_ortho.bottom,
                m_ortho.top, m_ortho.front, m_ortho.back);
        }
    }
}
