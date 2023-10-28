#include "Camera.hpp"
#include "hepch.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
namespace HAIEngine
{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

    void CameraController::ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVec();
    }

    void CameraController::updateView()
    {
        // rotation
        glm::mat4 rotM = glm::mat4(1.0f);
        {
            rotM = glm::rotate(rotM, glm::radians(m_direction.x * (m_flipY ? -1.0f : 1.0f)), { 1.0f, 0.0f, 0.0f });
            rotM = glm::rotate(rotM, glm::radians(m_direction.y), { 0.0f, 1.0f, 0.0f });
            rotM = glm::rotate(rotM, glm::radians(m_direction.z), { 0.0f, 0.0f, 1.0f });
        }

        // translation
        glm::mat4 transM = glm::mat4(1.0f);
        {
            glm::vec3 translation = -m_position;
            if (m_flipY)
            {
                translation.y *= -1.0f;
            }

            transM = glm::translate(transM, translation);
        }

        if (m_camera->m_cameraType == CameraType::PERSPECTIVE)
        {
            m_camera->m_view = glm::lookAt(m_position, m_position + m_Front, m_Up);
        }
        else if (m_camera->m_cameraType == CameraType::ORTHO)
        {
            m_camera->m_view = transM * rotM;
        }
    };

    void CameraController::update(float deltaTime)
    {
        updateView();
        updateProj();
        if (m_camera->m_cameraType == CameraType::PERSPECTIVE)
        {
            if (std::any_of(m_directions.begin(), m_directions.end(), [](const auto& key) -> bool { return key.second; }))
            {
                UpdateCameraVec();

                float moveSpeed{ deltaTime * m_movementSpeed };

                if (m_directions[Direction::UP])
                    m_position += m_Front * moveSpeed;
                if (m_directions[Direction::DOWN])
                    m_position -= m_Front * moveSpeed;
                if (m_directions[Direction::LEFT])
                    m_position -= m_Right * moveSpeed;
                if (m_directions[Direction::RIGHT])
                    m_position += m_Right * moveSpeed;
            }
        }
    };

    void CameraController::updateProj()
    {
        if (m_camera->m_cameraType == CameraType::PERSPECTIVE)
        {
            m_camera->m_projection = glm::perspective(glm::radians(m_camera->m_perspective.fov), m_camera->m_aspect,
                m_camera->m_perspective.znear, m_camera->m_perspective.zfar);
            if (m_flipY)
            {
                m_camera->m_projection[1][1] *= -1.0f;
            }
        }
        else if (m_camera->m_cameraType == CameraType::ORTHO)
        {
            m_camera->m_projection = glm::ortho(m_camera->m_ortho.left, m_camera->m_ortho.right, m_camera->m_ortho.bottom,
                m_camera->m_ortho.top, m_camera->m_ortho.front, m_camera->m_ortho.back);
        }
    }

    void CameraController::UpdateCameraVec()
    {
        // from learn opengl
        glm::vec3 tfront;
        tfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        tfront.y = sin(glm::radians(pitch));
        tfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        m_Front = glm::normalize(tfront);
        // also re-calculate the Right and Up vector
        m_Right = glm::normalize(glm::cross(m_Front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
}
