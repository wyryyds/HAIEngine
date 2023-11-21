#pragma once
#include "ECS/Components/Camera.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <variant>

namespace HAIEngine
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

    struct EditorCamera
    {
        EditorCamera(CameraType cameraType) : m_cameraType(cameraType) {};
        ~EditorCamera() = default;

        CameraType m_cameraType{ CameraType::UNDEFINED };

        float     m_aspect{ 0.0f };
        glm::mat4 m_projection{ 1.0f };
        glm::mat4 m_view{ 1.0f };

        std::variant<orthoParams, perspectiveParams> m_cameraDatas;
    };

    enum class Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    class CameraController
    {
    public:
        static std::unique_ptr<CameraController> Create(EditorCamera* camera)
        {
            auto instance = std::unique_ptr<CameraController>(new CameraController(camera));
            return instance;
        }

        static EditorCamera* CreatePerspectiveCamera(CameraType type, float aspectRatio, float fov, float znear, float zfar)
        {
            EditorCamera* camera = new EditorCamera(type);
            camera->m_aspect = aspectRatio;
            camera->m_cameraDatas = perspectiveParams{fov, znear, zfar};
            return camera;
        }

        inline glm::vec3 GetCameraPosition(){ return m_position; }

        void move(Direction direction, bool flag) { m_directions[direction] = flag; }

        void rotate(glm::vec3 delta)
        {
            if (!enabled)
            {
                return;
            }
            m_direction += delta * m_rotationSpeed;
        }
        void ProcessMouseMovement(float xoffset, float yoffset);
        void translate(glm::vec3 delta) { m_position += delta * m_movementSpeed; }
        void update(float deltaTime);
        void setCursorEnabled(bool flag) { enabled = flag; }

    private:
        CameraController(EditorCamera* camera) : m_camera{ camera } {}
        void updateProj();
        void updateView();

        EditorCamera* m_camera{};

        bool      m_flipY{ false };
        glm::vec3 m_direction{ 0.0f, 0.0f, 0.0f };
        glm::vec3 m_position{ 0.0f, 0.0f, 6.0f };
        glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
        glm::vec3 m_Right{1.0f, 0.0f, 0.0f };
        glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
        float     yaw = -90.0f;
        float     pitch = 0.0f;
        float     sensitivity = 0.1f;
        float     zoom = 45.0f;
        float     m_rotationSpeed{ 0.1f };
        float     m_movementSpeed{ 5.0f };

        std::unordered_map<Direction, bool> m_directions{
            {Direction::LEFT, false}, {Direction::RIGHT, false}, {Direction::UP, false}, {Direction::DOWN, false} };

        bool enabled = false;

        void UpdateCameraVec();
    };
}