#include "Camera.hpp"

#include "Core/Log.hpp"
#include "Core/Reflection.hpp"
#include "HAIEngine/ECS/Components/Transform.hpp"
#include "HAIEngine/ECS/GameObject.hpp"
#include "hepch.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace HAIEngine
{
    //REFLECTION(Camera, Component);

    Camera::Camera(CameraType type, const std::variant<orthoParams, perspectiveParams>& params)
        : Component("Camera"), m_cameraType(type), m_cameraDatas(params)
    {
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
            m_position = GetTransform()->GetPosition();
            m_Front = GetTransform()->GetFront();
            m_Up = GetTransform()->GetUp();

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
            m_projection = glm::perspective(glm::radians(std::get<perspectiveParams>(m_cameraDatas).fov), std::get<perspectiveParams>(m_cameraDatas).m_aspect,
                std::get<perspectiveParams>(m_cameraDatas).znear, std::get<perspectiveParams>(m_cameraDatas).zfar);
        }
        else if (m_cameraType == CameraType::ORTHO)
        {
            m_projection = glm::ortho(
                std::get<orthoParams>(m_cameraDatas).left, std::get<orthoParams>(m_cameraDatas).right,
                std::get<orthoParams>(m_cameraDatas).bottom, std::get<orthoParams>(m_cameraDatas).top, 
                std::get<orthoParams>(m_cameraDatas).front, std::get<orthoParams>(m_cameraDatas).back);
        }
    }

    json Camera::Serialize()
    {
        json resjson;
        resjson["type"] = SerializeHelper::SerializeData(m_typeName);
        resjson["guid"] = SerializeHelper::SerializeData(m_guid);

        switch (m_cameraType) 
        {
            case CameraType::UNDEFINED:
                resjson["cameraType"] = SerializeHelper::SerializeData("UNDEFINED");
                break;
            case CameraType::ORTHO:
                resjson["cameraType"] = SerializeHelper::SerializeData("ORTHO");
                break;
            case CameraType::PERSPECTIVE:
                resjson["cameraType"] = SerializeHelper::SerializeData("PERSPECTIVE");
                resjson["aspectRatio"] = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraDatas).m_aspect);
                resjson["fov"] = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraDatas).fov);
                resjson["znear"] = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraDatas).znear);
                resjson["zfar"] = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraDatas).zfar);
                break;
            default:
                resjson["cameraType"] = "UNDEFINED";
                break;
        }

        return resjson;
    }

    void Camera::DeSerialize(const json& jsonData)
    {
        if (SerializeHelper::DeSerializeData<std::string>(jsonData["type"]) != m_typeName)
        {
            LOG_Error("type not match!");
            return;
        }

        m_guid = SerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

        if (SerializeHelper::DeSerializeData<std::string>(jsonData["cameraType"]) == "UNDEFINED")
            return;

        if (SerializeHelper::DeSerializeData<std::string>(jsonData["cameraType"]) == "PERSPECTIVE")
        {
            m_cameraType = CameraType::PERSPECTIVE;

            float aspect = SerializeHelper::DeSerializeData<float>(jsonData["aspectRatio"]);
            float fov = SerializeHelper::DeSerializeData<float>(jsonData["fov"]);
            float znear = SerializeHelper::DeSerializeData<float>(jsonData["znear"]);
            float zfar = SerializeHelper::DeSerializeData<float>(jsonData["zfar"]);

            m_cameraDatas = perspectiveParams{ aspect, fov, znear, zfar };
            return;
        }

        if(SerializeHelper::DeSerializeData<std::string>(jsonData["cameraType"]) == "ORTHO")
        {
            // TODO
            return;
        }

    }
}
