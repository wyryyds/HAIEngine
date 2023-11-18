#pragma once
#include "HAIEngine/ECS/Component.hpp"
#include <variant>

namespace HAIEngine
{
	enum class LightType
	{
		DIRECTION,
		POINT,
		SPOT
	};

	class Light : public Component
	{
		struct directionParams
		{
			float m_ambientIntensity = 0.1f;
			float m_intensity = 0.5f;
			glm::vec3 m_color{ 1.0f };
		};
		struct pointParams
		{
			float m_ambientIntensity = 0.1f;
			float m_intensity = 0.5f;
			glm::vec3 m_color{ 1.0f };
			float range = 1.0f;
		};
		struct spotParams
		{
			float m_ambientIntensity = 0.1f;
			float m_intensity = 0.5f;
			glm::vec3 m_color{ 1.0f };
			float range = 1.0f;
			float spotRange = 1.0f;
		};

	public:
		Light() : Component("Light") {}
		virtual ~Light() override = default;

		inline void SetAmbientIntensity(float intensity) { m_ambientIntensity = std::clamp(intensity, 0.0f, 1.0f); }
		inline void SetIntensity(float intensity) { m_intensity = std::max(intensity, 0.0f); }
		inline void SetColor(glm::vec3 color) { m_color = color; }
		
		inline float GetAmbientIntensity() const { return m_ambientIntensity; }
		inline float GetIntensity() const { return m_intensity; }
		inline glm::vec3 GetColor() const { return m_color; }

		json Serialize() override;
		void DeSerialize(const json& jsonData) override;

	public:
		LightType m_lightType;
		std::variant<directionParams, pointParams, spotParams> m_lightDatas;

	private:
		float m_ambientIntensity = 0.1f;
		float m_intensity = 0.5f;
		glm::vec3 m_color{ 1.0f };
	};

}