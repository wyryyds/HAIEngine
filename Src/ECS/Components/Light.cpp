#include "Light.hpp"
#include "hepch.hpp"
#include "ECS/GameObject.hpp"
#include "ECS/Components/Transform.hpp"
#include "Core/Reflection.hpp"

namespace HAIEngine
{
	//REFLECTION(Light, Component);

	Light::Light(LightType type, const std::variant<directionParams, pointParams, spotParams>& params)
		: m_lightType(type), Component("Light"), m_lightDatas(params)
	{
	}

	void Light::SetAmbientIntensity(float intensity)
	{
		std::visit([intensity](auto& params) { params.m_ambientIntensity = std::clamp(intensity, 0.0f, 1.0f); }, m_lightDatas);
	}

	void Light::SetIntensity(float intensity)
	{
		std::visit([intensity](auto& params) { params.m_intensity = std::max(intensity, 0.0f); }, m_lightDatas);
	}

	void Light::SetColor(glm::vec3 color)
	{
		std::visit([color](auto& params) { params.m_color = color; }, m_lightDatas);
	}

	float Light::GetAmbientIntensity()
	{
		return std::visit([](auto& params) { return params.m_ambientIntensity; }, m_lightDatas);
	}

	float Light::GetIntensity()
	{
		return std::visit([](auto& params) { return params.m_intensity; }, m_lightDatas);
	}

	glm::vec3 Light::GetColor()
	{
		return std::visit([](auto& params) { return params.m_color; }, m_lightDatas);
	}

	const glm::vec3 Light::GetDirectionNormal()
	{
		return glm::normalize(m_fatherGO->m_transform->m_rotation);
	}

	const float Light::GetAttenuation(glm::vec3 position) const
	{
		if (auto* params = std::get_if<pointParams>(&m_lightDatas))
		{
			float dis = glm::distance(position, m_fatherGO->m_transform->m_position);
			return params->m_intensity / (params->constant + params->linear * dis + params->quadratic * dis * dis);
		}
		else if (auto* params = std::get_if<spotParams>(&m_lightDatas))
		{
			// TODO
		}
	}

	json Light::Serialize()
	{
		return json();
	}

	void Light::DeSerialize(const json& jsonData)
	{

	}

	void Light::GuiDisplay()
	{

	}
}