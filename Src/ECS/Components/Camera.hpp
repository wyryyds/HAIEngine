#pragma once
#include"ECS/Component.hpp"

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
        Count,
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
            float m_aspect{ 0.0f };
            float fov{ 60.0f };
            float znear{ 96.0f };
            float zfar{ 0.01f };
        };

    public:
        Camera() : Component("Camera") {}
        Camera(CameraType type, const std::variant<orthoParams, perspectiveParams>& params);
        ~Camera() = default;

        inline glm::mat4 GetViewMatrix() const { return m_view; }
        inline glm::mat4 GetProjectionMatrix() const { return m_projection; }
        inline glm::mat4 GetViewProjection() const { return m_projection * m_view; }

        void Update(TimeStep ts) override;
        virtual json Serialize() override;
        virtual void DeSerialize(const json& jsondata) override;
        virtual void GuiDisplay() override;
    public:
        CameraType m_cameraType{ CameraType::UNDEFINED };
     
        std::variant<orthoParams, perspectiveParams> m_cameraDatas;

    private:
        void UpdateView();
        void UpdateProjection();

    private:
        glm::vec3 m_position{ 0.0f, 0.0f, 6.0f };
        glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };

        glm::mat4 m_projection{ 1.0f };
        glm::mat4 m_view{ 1.0f };
    };
}