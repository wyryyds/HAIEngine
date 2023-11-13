#include "Camera.hpp"
#include "hepch.hpp"
#include "Core/Log.hpp"
#include "HAIEngine/ECS/GameObject.hpp"
#include "HAIEngine/ECS/Components/Transform.hpp"
#include "Core/Reflection.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace HAIEngine
{
    REFLECTION(Camera, Component);

    Camera::Camera(CameraType cameraType, float aspectRatio, float fov, float znear, float zfar)
        : Component("Camera"), m_cameraType(cameraType), m_aspect(aspectRatio)
    {
        m_cameraParams = perspectiveParams{fov, znear, zfar};
    }

    void Camera::Update(TimeStep ts)
    {
        UpdateView();
        UpdateProjection();
    }

    void Camera::UpdateView()
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

    void Camera::UpdateProjection()
    {
        if (m_cameraType == CameraType::PERSPECTIVE)
        {
            m_projection = glm::perspective(glm::radians(std::get<perspectiveParams>(m_cameraParams).fov), m_aspect,
                std::get<perspectiveParams>(m_cameraParams).znear, std::get<perspectiveParams>(m_cameraParams).zfar);
        }
        else if (m_cameraType == CameraType::ORTHO)
        {
            m_projection = glm::ortho(
                std::get<orthoParams>(m_cameraParams).left, std::get<orthoParams>(m_cameraParams).right,
                std::get<orthoParams>(m_cameraParams).bottom, std::get<orthoParams>(m_cameraParams).top, 
                std::get<orthoParams>(m_cameraParams).front, std::get<orthoParams>(m_cameraParams).back);
        }
    }

    json Camera::Serialize(const std::string& name)
    {
        json resjson;
        resjson["type"] = name;
        resjson["guid"] = m_guid;

        switch (m_cameraType) 
        {
            case CameraType::UNDEFINED:
                resjson["cameraType"] = "UNDEFINED";
                break;
            case CameraType::ORTHO:
                resjson["cameraType"] = "ORTHO";
                break;
            case CameraType::PERSPECTIVE:
                resjson["cameraType"] = "PERSPECTIVE";
                resjson["aspectRatio"] = std::to_string(m_aspect);
                resjson["fov"] = std::to_string(std::get<perspectiveParams>(m_cameraParams).fov);
                resjson["znear"] = std::to_string(std::get<perspectiveParams>(m_cameraParams).znear);
                resjson["zfar"] = std::to_string(std::get<perspectiveParams>(m_cameraParams).zfar);
                break;
            default:
                resjson["cameraType"] = "UNDEFINED";
                break;
        }

        return resjson;
    }

    void Camera::DeSerialize(const json& jsondata)
    {
        if (jsondata["type"].get<std::string>() != m_typeName)
        {
            LOG_Error("type not match!");
            return;
        }
        m_guid = jsondata["guid"];

        if (jsondata["cameraType"].get<std::string>() == "UNDEFINED")
            return;

        if (jsondata["cameraType"].get<std::string>() == "PERSPECTIVE")
        {
            m_cameraType = CameraType::PERSPECTIVE;

            m_aspect = std::stof(jsondata["aspectRatio"].get<std::string>());
            float fov = std::stof(jsondata["fov"].get<std::string>());
            float znear = std::stof(jsondata["znear"].get<std::string>());
            float zfar = std::stof(jsondata["zfar"].get<std::string>());

            m_cameraParams = perspectiveParams{ fov, znear, zfar };
            return;
        }

        if(jsondata["cameraType"].get<std::string>() == "ORTHO")
        {
            // TODO
            return;
        }

    }
}
