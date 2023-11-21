#include "EditorCamera.hpp"
#include "hepch.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace HAIEngine
{
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
        UpdateCameraVec();
        updateView();
        updateProj();
        if (m_camera->m_cameraType == CameraType::PERSPECTIVE)
        {
            if (std::any_of(m_directions.begin(), m_directions.end(), [](const auto& key) -> bool { return key.second; }))
            {
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
            m_camera->m_projection = glm::perspective(glm::radians(std::get<perspectiveParams>(m_camera->m_cameraDatas).fov), 
                m_camera->m_aspect, std::get<perspectiveParams>(m_camera->m_cameraDatas).znear, 
                std::get<perspectiveParams>(m_camera->m_cameraDatas).zfar);
        }
        else if (m_camera->m_cameraType == CameraType::ORTHO)
        {
            m_camera->m_projection = glm::ortho(
                std::get<orthoParams>(m_camera->m_cameraDatas).left, std::get<orthoParams>(m_camera->m_cameraDatas).right, 
                std::get<orthoParams>(m_camera->m_cameraDatas).bottom, std::get<orthoParams>(m_camera->m_cameraDatas).top, 
                std::get<orthoParams>(m_camera->m_cameraDatas).front, std::get<orthoParams>(m_camera->m_cameraDatas).back);
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
}
