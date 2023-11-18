#pragma once
#include"HAIEngine/ECS/Component.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <variant>

namespace HAIEngine
{
    class GameObject;

    enum class CameraType
    {
        UNDEFINED,
        ORTHO,
        PERSPECTIVE,
    };

    class Camera : public Component
    {
        struct orthoParams
        {
            float left{};
            float right{};
            float bottom{};
            float top{};
            float front{};
            float back{};
        };
        struct perspectiveParams
        {
            float fov{ 60.0f };
            float znear{ 96.0f };
            float zfar{ 0.01f };
        };

    public:
        Camera() : Component("Camera") {}
        // TODO: init as ortho camera
        //Camera(CameraType cameraType, float left, float right, float bottom, float top);
        Camera(CameraType cameraType, float aspectRatio, float fov, float znear, float zfar);
        ~Camera() = default;

        inline glm::mat4 GetViewMatrix() { return m_view; }
        inline glm::mat4 GetProjectionMatrix() { return m_projection; }
        inline glm::mat4 GetViewProjection() { return m_projection * m_view; }

        void Update(TimeStep ts) override;
        json Serialize() override;
        void DeSerialize(const json& jsondata) override;
    public:
        CameraType m_cameraType{ CameraType::UNDEFINED };
     
        std::variant<orthoParams, perspectiveParams> m_cameraDatas;

    private:
        void UpdateView();
        void UpdateProjection();
    private:
        float m_aspect{ 0.0f };
        glm::vec3 m_position{ 0.0f, 0.0f, 6.0f };
        glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };

        glm::mat4 m_projection{ 1.0f };
        glm::mat4 m_view{ 1.0f };
    };
}