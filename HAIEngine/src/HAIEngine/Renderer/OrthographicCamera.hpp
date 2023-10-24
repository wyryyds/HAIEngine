#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

namespace HAIEngine
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};



	enum class CameraType
	{
		UNDEFINED,
		ORTHO,
		PERSPECTIVE,
	};

    struct Camera
    {
        Camera(CameraType cameraType) : m_cameraType(cameraType) {};
        ~Camera() = default;

        CameraType m_cameraType{ CameraType::UNDEFINED };

        float     m_aspect{ 0.0f };
        glm::mat4 m_projection{ 1.0f };
        glm::mat4 m_view{ 1.0f };

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
        static std::unique_ptr<CameraController> Create(Camera* camera)
        {
            auto instance = std::unique_ptr<CameraController>(new CameraController(camera));
            return instance;
        }

        static Camera* CreatePerspectiveCamera(CameraType type, float aspectRatio, float fov, float znear, float zfar)
        {
            Camera* camera = new Camera(type);
            camera->m_aspect = aspectRatio;
            camera->m_perspective.zfar = zfar;
            camera->m_perspective.znear = znear;
            camera->m_perspective.fov = fov;
            return camera;
        }

        void move(Direction direction, bool flag) { m_directions[direction] = flag; }

        void rotate(glm::vec3 delta)
        {
            if (!enabled)
            {
                return;
            }
            m_direction += delta * m_rotationSpeed;
        }
        void translate(glm::vec3 delta) { m_position += delta * m_movementSpeed; }
        void update(float deltaTime);
        void setCursorEnabled(bool flag) { enabled = flag; }

    private:
        CameraController(Camera* camera) : m_camera{ camera } {}
        void updateProj();
        void updateView();

        Camera* m_camera{};

        bool      m_flipY{ true };
        glm::vec3 m_direction{ 0.0f, 180.0f, 0.0f };
        glm::vec3 m_position{ 0.0f, 0.0f, -3.0f };
        float     m_rotationSpeed{ 0.1f };
        float     m_movementSpeed{ 2.5f };

        std::unordered_map<Direction, bool> m_directions{
            {Direction::LEFT, false}, {Direction::RIGHT, false}, {Direction::UP, false}, {Direction::DOWN, false} };

        bool enabled = false;
    };
}