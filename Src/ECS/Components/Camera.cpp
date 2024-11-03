#include "Camera.hpp"
#include "Core/Log.hpp"
#include "Core/Reflection.hpp"
#include "Editor/GuiHelper.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/GameObject.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace HAIEngine
{
    //REFLECTION(Camera, Component);

    Camera::Camera(CameraType type, const std::variant<orthoParams, perspectiveParams>& params)
        : Component("Camera"), m_cameraType(type), m_cameraData(params)
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
            auto transform = this->GetTransform();
            m_position = transform.GetPosition();
            m_Front = transform.GetFront();
            m_Up = transform.GetUp();

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
            m_projection = glm::perspective(glm::radians(std::get<perspectiveParams>(m_cameraData).fov), std::get<perspectiveParams>(m_cameraData).m_aspect,
                std::get<perspectiveParams>(m_cameraData).zNear, std::get<perspectiveParams>(m_cameraData).zFar);
        }
        else if (m_cameraType == CameraType::ORTHO)
        {
            m_projection = glm::ortho(
                std::get<orthoParams>(m_cameraData).left, std::get<orthoParams>(m_cameraData).right,
                std::get<orthoParams>(m_cameraData).bottom, std::get<orthoParams>(m_cameraData).top, 
                std::get<orthoParams>(m_cameraData).front, std::get<orthoParams>(m_cameraData).back);
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
                resjson["cameraType"]  = SerializeHelper::SerializeData("PERSPECTIVE");
                resjson["aspectRatio"] = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraData).m_aspect);
                resjson["fov"]         = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraData).fov);
                resjson["znear"]       = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraData).zNear);
                resjson["zfar"]        = SerializeHelper::SerializeData(std::get<perspectiveParams>(m_cameraData).zFar);
                break;
            default:
                resjson["cameraType"] = "UNDEFINED";
                break;
        }

        return resjson;
    }

    void Camera::DeSerialize(const json& jsonData)
    {
        if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["type"]) != m_typeName)
        {
            LOG_Error("type not match!");
            return;
        }

        m_guid = JsonSerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

        if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["cameraType"]) == "UNDEFINED")
            return;

        if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["cameraType"]) == "PERSPECTIVE")
        {
            m_cameraType = CameraType::PERSPECTIVE;

            float aspect = JsonSerializeHelper::DeSerializeData<float>(jsonData["aspectRatio"]);
            float fov    = JsonSerializeHelper::DeSerializeData<float>(jsonData["fov"]);
            float znear  = JsonSerializeHelper::DeSerializeData<float>(jsonData["znear"]);
            float zfar   = JsonSerializeHelper::DeSerializeData<float>(jsonData["zfar"]);

            m_cameraData = perspectiveParams{ aspect, fov, znear, zfar };
            return;
        }

        if(JsonSerializeHelper::DeSerializeData<std::string>(jsonData["cameraType"]) == "ORTHO")
        {
            // TODO
            return;
        }

    }

    void Camera::GuiDisplay()
    {
        GuiHelper::DrawEnumControl(m_cameraType);
        switch (m_cameraType)
        {
        case CameraType::UNDEFINED:
            break;
        case CameraType::ORTHO:
            // TODO
            break;
        case CameraType::PERSPECTIVE:
            GuiHelper::DrawFloatSlider("Fov", &std::get<perspectiveParams>(m_cameraData).fov, 0.0f, 90.0f);
            GuiHelper::DrawFloatInput("ZNear", &std::get<perspectiveParams>(m_cameraData).zNear);
            GuiHelper::DrawFloatInput("ZFar", &std::get<perspectiveParams>(m_cameraData).zFar);
            break;
        default:
            break;
        }
    }
}
