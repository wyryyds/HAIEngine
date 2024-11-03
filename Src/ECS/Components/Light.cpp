#include "Light.hpp"
#include "hepch.hpp"
#include "ECS/GameObject.hpp"
#include "ECS/Components/Transform.hpp"
#include "Editor/GuiHelper.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace HAIEngine
{
	Light::Light(LightType type, const std::variant<directionParams, pointParams, spotParams>& params)
		: Component("Light"), m_lightType(type), m_lightData(params)
	{
	}
	
	void Light::SetAmbientIntensity(float intensity)
	{
		std::visit([intensity](auto& params) { params.ambientIntensity = std::clamp(intensity, 0.0f, 1.0f); }, m_lightData);
	}

	void Light::SetIntensity(float intensity)
	{
		std::visit([intensity](auto& params) { params.intensity = std::max(intensity, 0.0f); }, m_lightData);
	}

	void Light::SetColor(glm::vec3 color)
	{
		std::visit([color](auto& params) { params.color = color; }, m_lightData);
	}

	float Light::GetAmbientIntensity()
	{
		return std::visit([](auto& params) { return params.ambientIntensity; }, m_lightData);
	}

	float Light::GetIntensity()
	{
		return std::visit([](auto& params) { return params.intensity; }, m_lightData);
	}

	glm::vec3 Light::GetColor()
	{
		return std::visit([](auto& params) { return params.color; }, m_lightData);
	}

	glm::vec3 Light::GetDirectionNormal() const
	{
		return glm::normalize(m_fatherGO->m_transform->m_rotation);
	}

	float Light::GetAttenuation(glm::vec3 position) const
	{
		if (auto* pParams = std::get_if<pointParams>(&m_lightData))
		{
			float dis = glm::distance(position, m_fatherGO->m_transform->m_position);
			return pParams->intensity / (pParams->constant + pParams->linear * dis + pParams->quadratic * dis * dis);
		}
		
		if (auto* sParams = std::get_if<spotParams>(&m_lightData))
		{
			// TODO
			return 0.f;
		}

		return 0.f;
	}

	json Light::Serialize()
	{
		json resJson;
		resJson["type"] = SerializeHelper::SerializeData(m_typeName);
		resJson["guid"] = SerializeHelper::SerializeData(m_guid);

		switch (m_lightType)
		{
		case HAIEngine::LightType::UNDEFINED:
			resJson["lightType"] = SerializeHelper::SerializeData("UNDEFINED");
			break;
		case HAIEngine::LightType::DIRECTION:
			resJson["lightType"]		= SerializeHelper::SerializeData("DIRECTION");
			resJson["ambientIntensity"] = SerializeHelper::SerializeData(std::get<directionParams>(m_lightData).ambientIntensity);
			resJson["intensity"]		= SerializeHelper::SerializeData(std::get<directionParams>(m_lightData).intensity);
			resJson["color"]			= SerializeHelper::SerializeData(std::get<directionParams>(m_lightData).color);
			break;
		case HAIEngine::LightType::POINT:
			resJson["lightType"]	    = SerializeHelper::SerializeData("POINT");
			resJson["ambientIntensity"] = SerializeHelper::SerializeData(std::get<pointParams>(m_lightData).ambientIntensity);
			resJson["intensity"]		= SerializeHelper::SerializeData(std::get<pointParams>(m_lightData).intensity);
			resJson["color"]			= SerializeHelper::SerializeData(std::get<pointParams>(m_lightData).color);
			resJson["range"]			= SerializeHelper::SerializeData(std::get<pointParams>(m_lightData).range);
			break;
		case HAIEngine::LightType::SPOT:
			resJson["lightType"]		= SerializeHelper::SerializeData("SPOT");
			resJson["ambientIntensity"] = SerializeHelper::SerializeData(std::get<spotParams>(m_lightData).ambientIntensity);
			resJson["intensity"]		= SerializeHelper::SerializeData(std::get<spotParams>(m_lightData).intensity);
			resJson["color"]			= SerializeHelper::SerializeData(std::get<spotParams>(m_lightData).color);
			resJson["range"]			= SerializeHelper::SerializeData(std::get<spotParams>(m_lightData).range);
			break;
		case HAIEngine::LightType::Count:
			break;
		}

		return resJson;
	}

	void Light::DeSerialize(const json& jsonData)
	{
		if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["type"]) != m_typeName)
		{
			LOG_Error("type not match!");
			return;
		}

		m_guid = JsonSerializeHelper::DeSerializeData<size_t>(jsonData["guid"]);

		if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "UNDEFINED")
			return;

		if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "DIRECTION")
		{
			m_lightType = LightType::DIRECTION;
			auto ambientIntensity = JsonSerializeHelper::DeSerializeData<float>(jsonData["ambientIntensity"]);
			auto intensity        = JsonSerializeHelper::DeSerializeData<float>(jsonData["intensity"]);
			auto color			  = JsonSerializeHelper::DeSerializeData<glm::vec3>(jsonData["color"]);

			m_lightData = directionParams{ ambientIntensity, intensity, color };
			return;
		}

		if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "POINT")
		{
			m_lightType = LightType::POINT;
			auto ambientIntensity = JsonSerializeHelper::DeSerializeData<float>(jsonData["ambientIntensity"]);
			auto intensity		  = JsonSerializeHelper::DeSerializeData<float>(jsonData["intensity"]);
			auto color			  = JsonSerializeHelper::DeSerializeData<glm::vec3>(jsonData["color"]);
			auto range			  = JsonSerializeHelper::DeSerializeData<float>(jsonData["range"]);

			m_lightData = pointParams{ ambientIntensity, intensity, color, range };
			return;
		}

		if (JsonSerializeHelper::DeSerializeData<std::string>(jsonData["lightType"]) == "SPOT")
		{
			m_lightType = LightType::SPOT;
			auto ambientIntensity = JsonSerializeHelper::DeSerializeData<float>(jsonData["ambientIntensity"]);
			auto intensity		  = JsonSerializeHelper::DeSerializeData<float>(jsonData["intensity"]);
			auto color			  = JsonSerializeHelper::DeSerializeData<glm::vec3>(jsonData["color"]);
			auto range			  = JsonSerializeHelper::DeSerializeData<float>(jsonData["range"]);

			m_lightData = spotParams{ ambientIntensity, intensity, color, range };
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
			GuiHelper::DrawColorEditor3("Color", glm::value_ptr(std::get<directionParams>(m_lightData).color));
			GuiHelper::DrawFloatInput("AmbientIntensity", &std::get<directionParams>(m_lightData).ambientIntensity);
			GuiHelper::DrawFloatInput("Intensity", &std::get<directionParams>(m_lightData).intensity);
			break;
		case HAIEngine::LightType::POINT:
			GuiHelper::DrawColorEditor3("Color", glm::value_ptr(std::get<pointParams>(m_lightData).color));
			GuiHelper::DrawFloatInput("AmbientIntensity", &std::get<pointParams>(m_lightData).ambientIntensity);
			GuiHelper::DrawFloatInput("Intensity", &std::get<pointParams>(m_lightData).intensity);
			GuiHelper::DrawFloatInput("Range", &std::get<pointParams>(m_lightData).range);
			break;
		case HAIEngine::LightType::SPOT:
			GuiHelper::DrawColorEditor3("Color", glm::value_ptr(std::get<spotParams>(m_lightData).color));
			GuiHelper::DrawFloatInput("AmbientIntensity", &std::get<spotParams>(m_lightData).ambientIntensity);
			GuiHelper::DrawFloatInput("Intensity", &std::get<spotParams>(m_lightData).intensity);
			GuiHelper::DrawFloatInput("Range", &std::get<spotParams>(m_lightData).range);
			break;
		case HAIEngine::LightType::Count:
			break;
		}
	}
}