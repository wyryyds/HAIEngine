#pragma once
#include"HAIEngine/ECS/Component.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

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
    public:
        Camera() = default;
        // TODO: init as ortho camera
        //Camera(CameraType cameraType, float left, float right, float bottom, float top);
        Camera(CameraType cameraType, float aspectRatio, float fov, float znear, float zfar);
        ~Camera() = default;

        inline glm::mat4 GetViewMatrix() { return m_view; }
        inline glm::mat4 GetProjectionMatrix() { return m_projection; }

        void Update() override;
        json Serialize(const std::string& name) override;
        void DeSerialize(const json& jsondata) override;
    public:
        CameraType m_cameraType{ CameraType::UNDEFINED };
        union
        {
            struct
            {
                float left{};
                float right{};
                float bottom{};
                float top{};
                float front{};
                float back{};
            } m_ortho;
            struct
            {
                float fov{ 60.0f };
                float znear{ 96.0f };
                float zfar{ 0.01f };
            } m_perspective;
        };

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