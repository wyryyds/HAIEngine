#include "Light.hpp"
#include "hepch.hpp"
#include "ECS/GameObject.hpp"
#include "ECS/Components/Transform.hpp"
#include "Core/Reflection.hpp"
#include "Editor/GuiHelper.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace HAIEngine
{
	Light::Light(LightType type, const std::variant<directionParams, pointParams, spotParams>& params)
		: m_lightType(type), Component("Light"), m_lightDatas(params)
	{
	}
	
	void Light::SetAmbientIntensity(float intensity)
	{
		std::visit([intensity](auto& params) { params.ambientIntensity = std::clamp(intensity, 0.0f, 1.0f); }, m_lightDatas);
	}

	void Light::SetIntensity(float intensity)
	{
		std::visit([intensity](auto& params) { params.intensity = std::max(intensity, 0.0f); }, m_lightDatas);
	}

	void Light::SetColor(glm::vec3 color)
	{
		std::visit([color](auto& params) { params.color = color; }, m_lightDatas);
	}

	float Light::GetAmbientIntensity()
	{
		return std::visit([](auto& params) { return params.ambientIntensity; }, m_lightDatas);
	}

	float Light::GetIntensity()
	{
		return std::visit([](auto& params) { return params.intensity; }, m_lightDatas);
	}

	glm::vec3 Light::GetColor()
	{
		return std::visit([](auto& params) { return params.color; }, m_lightDatas);
	}

	const glm::vec3 Light::GetDirectionNormal() const
	{
		return glm::normalize(m_fatherGO->m_transform->m_rotation);
	}

	const float Light::GetAttenuation(glm::vec3 position) const
	{
		if (auto* params = std::get_if<pointParams>(&m_lightDatas))
		{
			float dis = glm::distance(position, m_fatherGO->m_transform->m_position);
			return params->intensity / (params->constant + params->linear * dis + params->quadratic * dis * dis);
		}
		else if (auto* params = std::get_if<spotParams>(&m_lightDatas))
		{
			// TODO
		}
	}

	json Light::Serialize()
	{
		json resjson;
		resjson["type"] = SerializeHelper::SerializeData(m_typeName);
		resjson["guid"] = SerializeHelper::SerializeData(m_guid);

		switch (m_lightType)
		{
		case HAIEngine::LightType::UNDEFINED:
			resjson["lightType"] = SerializeHelper::SerializeData("UNDEFINED");
			break;
		case HAIEngine::LightType::DIRECTION:
			resjson["lightType"]		= SerializeHelper::SerializeData("DIRECTION");
			resjson["ambientIntensity"] = SerializeHelper::SerializeData(std::get<directionParams>(m_lightDatas).ambientIntensity);
			resjson["intensity"]		= SerializeHelper::SerializeData(std::get<directionParams>(m_lightDatas).intensity);
			resjson["color"]			= SerializeHelper::SerializeData(std::get<directionParams>(m_lightDatas).color);
			break;
		case HAIEngine::LightType::POINT:
			resjson["lightType"]	    = SerializeHelper::SerializeData("POINT");
			resjson["ambientIntensity"] = SerializeHelper::SerializeData(std::get<pointParams>(m_lightDatas).ambientIntensity);
			resjson["intensity"]		= SerializeHelper::SerializeData(std::get<pointParams>(m_lightDatas).intensity);
			resjson["color"]			= SerializeHelper::SerializeData(std::get<pointParams>(m_lightDatas).color);
			resjson["range"]			= SerializeHelper::SerializeData(std::get<pointParams>(m_lightDatas).range);
			break;
		case HAIEngine::LightType::SPOT:
			resjson["lightType"]		= SerializeHelper::SerializeData("SPOT");
			resjson["ambientIntensity"] = SerializeHelper::SerializeData(std::get<spotParams>(m_lightDatas).ambientIntensity);
			resjson["intensity"]		= SerializeHelper::SerializeData(std::get<spotParams>(m_lightDatas).intensity);
			resjson["color"]			= SerializeHelper::SerializeData(std::get<spotParams>(m_lightDatas).color);
			resjson["range"]			= SerializeHelper::SerializeData(std::get<spotParams>(m_lightDatas).range);
			break;
		case HAIEngine::LightType::Count:
			break;
		default:
			break;
		}

		return resjson;
	}

	void Light::DeSerialize(const json& jsonData)
	{
		if (SerializeHelper::DeSerializeData<std::string>(jsonData["type"]) != m_typeName)
		{
			LOG_Error("type not match!");
			return;
		}

		m_guid = SerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

		if (SerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "UNDEFINED")
			return;

		if (SerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "DIRECTION")
		{
			m_lightType = LightType::DIRECTION;
			auto ambientIntensity = SerializeHelper::DeSerializeData<float>(jsonData["ambientIntensity"]);
			auto intensity        = SerializeHelper::DeSerializeData<float>(jsonData["intensity"]);
			auto color			  = SerializeHelper::DeSerializeData<glm::vec3>(jsonData["color"]);

			m_lightDatas = directionParams{ ambientIntensity, intensity, color };
			return;
		}

		if (SerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "POINT")
		{
			m_lightType = LightType::POINT;
			auto ambientIntensity = SerializeHelper::DeSerializeData<float>(jsonData["ambientIntensity"]);
			auto intensity		  = SerializeHelper::DeSerializeData<float>(jsonData["intensity"]);
			auto color			  = SerializeHelper::DeSerializeData<glm::vec3>(jsonData["color"]);
			auto range			  = SerializeHelper::DeSerializeData<float>(jsonData["range"]);

			m_lightDatas = pointParams{ ambientIntensity, intensity, color, range };
			return;
		}

		if (SerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "SPOT")
		{
			m_lightType = LightType::SPOT;
			auto ambientIntensity = SerializeHelper::DeSerializeData<float>(jsonData["ambientIntensity"]);
			auto intensity		  = SerializeHelper::DeSerializeData<float>(jsonData["intensity"]);
			auto color			  = SerializeHelper::DeSerializeData<glm::vec3>(jsonData["color"]);
			auto range			  = SerializeHelper::DeSerializeData<float>(jsonData["range"]);

			m_lightDatas = spotParams{ ambientIntensity, intensity, color, range };
			return;
		}
	}

	void Light::GuiDisplay()
	{
		GuiHelper::DrawEnumControl(m_lightType);
		switch (m_lightType)
		{
		case HAIEngine::LightType::UNDEFINED:
			break;
		case HAIEngine::LightType::DIRECTION:
			GuiHelper::DrawColorEditor3("Color", glm::value_ptr(std::get<directionParams>(m_lightDatas).color));
			GuiHelper::DrawFloatInput("AmbientIntensity", &std::get<directionParams>(m_lightDatas).ambientIntensity);
			GuiHelper::DrawFloatInput("Intensity", &std::get<directionParams>(m_lightDatas).intensity);
			break;
		case HAIEngine::LightType::POINT:
			GuiHelper::DrawColorEditor3("Color", glm::value_ptr(std::get<pointParams>(m_lightDatas).color));
			GuiHelper::DrawFloatInput("AmbientIntensity", &std::get<pointParams>(m_lightDatas).ambientIntensity);
			GuiHelper::DrawFloatInput("Intensity", &std::get<pointParams>(m_lightDatas).intensity);
			GuiHelper::DrawFloatInput("Range", &std::get<pointParams>(m_lightDatas).range);
			break;
		case HAIEngine::LightType::SPOT:
			GuiHelper::DrawColorEditor3("Color", glm::value_ptr(std::get<spotParams>(m_lightDatas).color));
			GuiHelper::DrawFloatInput("AmbientIntensity", &std::get<spotParams>(m_lightDatas).ambientIntensity);
			GuiHelper::DrawFloatInput("Intensity", &std::get<spotParams>(m_lightDatas).intensity);
			GuiHelper::DrawFloatInput("Range", &std::get<spotParams>(m_lightDatas).range);
			break;
		case HAIEngine::LightType::Count:
			break;
		default:
			break;
		}
	}
}