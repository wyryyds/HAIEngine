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

	HAIEngine::Transform::Transform()
	{
		m_typeName = "Transform";
		m_position = glm::vec3(1.2, 1.3, 1.5);
		m_rotation = glm::vec3(2.1, 3.4, 4.6);
		m_scale	   = glm::vec3(2.66, 2.77, 3.88);
	}

	HAIEngine::Transform::Transform(glm::vec3 position)
	{
		m_typeName = "Transform";
		m_position = position;
		m_rotation = glm::vec3(0, 0, 0);
		m_scale    = glm::vec3(0, 0, 0);
	}

	glm::vec3 HAIEngine::Transform::GetFront()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.z));
		front.y = sin(glm::radians(m_rotation.z));
		front.z = sin(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.z));
		return glm::normalize(front);
	}

	glm::vec3 HAIEngine::Transform::GetRight()
	{
		return glm::normalize(glm::cross(GetFront(), Transform::up));
	}

	glm::vec3 HAIEngine::Transform::GetUp()
	{
		return glm::normalize(glm::cross(GetRight(), GetFront()));
	}

	glm::mat4 HAIEngine::Transform::GetModelMatrix()
	{
		glm::mat4 translationMatrix = glm::translate(glm::identity<glm::mat4>(), m_position);
		glm::mat4 rotationMatrix    = glm::eulerAngleYXZ(glm::radians(m_rotation.y), glm::radians(m_rotation.x), glm::radians(m_rotation.z));
		glm::mat4 scaleMatrix       = glm::scale(glm::identity<glm::mat4>(), m_scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	void HAIEngine::Transform::translate(glm::vec3 dir, float dis)
	{
		m_position += dis * dir;
	}

	json HAIEngine::Transform::Serialize(const std::string& name)
	{
		json resjson;
		resjson["type"] = name;
		resjson["guid"] = m_guid;
		// floatdata save as string
		std::string positionStr = std::to_string(m_position.x) + "," + std::to_string(m_position.y) + "," 
			+ std::to_string(m_position.z);
		std::string rotationStr = std::to_string(m_rotation.x) + "," + std::to_string(m_rotation.y) + "," 
			+ std::to_string(m_rotation.z);
		std::string scaleStr = std::to_string(m_scale.x) + "," + std::to_string(m_scale.y) + ","
			+ std::to_string(m_scale.z);

		resjson["position"] = positionStr;
		resjson["rotation"] = rotationStr;
		resjson["scale"] = scaleStr;

		return resjson;
	}

	void HAIEngine::Transform::DeSerialize(const json& jsondata)
	{
		if (jsondata["type"].get<std::string>() != "Transform")
		{
			LOG_Error("Component type is not match!");
			return;
		}
		m_guid = jsondata["guid"].get<size_t>();

		std::vector<std::string > stringdata;

		stringdata.push_back(jsondata["position"].get<std::string>());
		stringdata.push_back(jsondata["rotation"].get<std::string>());
		stringdata.push_back(jsondata["rotation"].get<std::string>());

		std::vector<std::vector<float> > floatValues;

		for (int i = 0; i < stringdata.size(); ++ i)
		{
			std::istringstream iss(stringdata[i]);
			std::string token;
			std::vector<float> subFloatValues;

			while (std::getline(iss, token, ',')) {
				float value = std::stof(token);
				subFloatValues.push_back(value);
			}

			floatValues.push_back(subFloatValues);
		}

		m_position = glm::vec3{ floatValues[0][0], floatValues[0][1], floatValues[0][2] };
		m_rotation = glm::vec3{ floatValues[1][0], floatValues[1][1], floatValues[1][2] };
		m_scale    = glm::vec3{ floatValues[2][0], floatValues[2][1], floatValues[2][2] };
	}
}
