#include "OrthographicCamera.h"
#include "hepch.h"
#include <glm/gtc/matrix_transform.hpp>
namespace HAIEngine
{

	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
		:m_ProjectionMatrix(glm::ortho(left, right, top, bottom, -1.0f, 1.0f))
	{
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
