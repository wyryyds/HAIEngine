#include "Transform.hpp"
#include "hepch.hpp"
#include "Core/Reflection.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace HAIEngine
{
	REFLECTION(Transform, Component);

	Transform::Transform()
		: Component("Transform")
	{
	}

	Transform::Transform(glm::vec3 position)
		: Component("Transform")
	{
		m_position = position;
	}

	glm::vec3 Transform::GetFront()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.z));
		front.y = sin(glm::radians(m_rotation.z));
		front.z = sin(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.z));
		return glm::normalize(front);
	}

	glm::vec3 Transform::GetRight()
	{
		return glm::normalize(glm::cross(GetFront(), Transform::up));
	}

	glm::vec3 Transform::GetUp()
	{
		return glm::normalize(glm::cross(GetRight(), GetFront()));
	}

	glm::mat4 Transform::GetModelMatrix()
	{
		glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), m_position);
		glm::mat4 rotationMatrix    = glm::eulerAngleYXZ(glm::radians(m_rotation.y), glm::radians(m_rotation.x), glm::radians(m_rotation.z));
		glm::mat4 scaleMatrix       = glm::scale(glm::identity<glm::mat4>(), m_scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	void Transform::translate(glm::vec3 dir, float dis)
	{
		m_position += dis * dir;
	}

	json Transform::Serialize(const std::string& name)
	{
		json resjson;
		resjson["type"] = SerializeHelper::SerializeData(name);
		resjson["guid"] = SerializeHelper::SerializeData(m_guid);
		resjson["position"] = SerializeHelper::SerializeData(m_position);
		resjson["rotation"] = SerializeHelper::SerializeData(m_rotation);
		resjson["scale"] = SerializeHelper::SerializeData(m_scale);

		return resjson;
	}

	void Transform::DeSerialize(const json& jsondata)
	{
		if (SerializeHelper::DeSerializeData<std::string>(jsondata["type"]) != m_typeName)
		{
			LOG_Error("Component type is not match!");
			return;
		}

		m_guid = SerializeHelper::DeSerializeData<size_t>(jsondata["guid"]);
		m_position = SerializeHelper::DeSerializeData<glm::vec3>(jsondata["position"]);
		m_rotation = SerializeHelper::DeSerializeData<glm::vec3>(jsondata["rotation"]);
		m_scale    = SerializeHelper::DeSerializeData<glm::vec3>(jsondata["scale"]);
	}
}
