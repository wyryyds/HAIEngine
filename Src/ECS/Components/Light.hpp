#pragma once
#include "ECS/Component.hpp"

#include <variant>

namespace HAIEngine
{
	enum class LightType : uint8_t
	{
		UNDEFINED,
		DIRECTION,
		POINT,
		SPOT,
		Count
	};

	class Light : public Component
	{
	public:
		struct directionParams
		{
			float ambientIntensity = 0.1f;
			float intensity = 0.5f;
			glm::vec3 color{ 1.0f };
		};
		struct pointParams
		{
			float ambientIntensity = 0.1f;
			float intensity = 0.5f;
			glm::vec3 color{ 1.0f };
			float range = 1.0f;
			float constant = 1.0f;
			float linear = 0.09f;
			float quadratic = 0.032f;
		};
		struct spotParams
		{
			float ambientIntensity = 0.1f;
			float intensity = 0.5f;
			glm::vec3 color{ 1.0f };
			float range = 1.0f;
			float spotRange = 1.0f;
		};

	public:
		Light() : Component("Light") {}
		Light(LightType type, const std::variant<directionParams, pointParams, spotParams>& params);
		Light(const Light& other) = default;
		Light(Light&& other) noexcept = default;
		Light& operator=(const Light& other) = default;
		Light& operator=(Light&& other) noexcept = default;
		virtual ~Light() override = default;
		virtual std::unique_ptr<Component> Clone() const override { return std::make_unique<Light>(*this); }

		void SetAmbientIntensity(float intensity);
		void SetIntensity(float intensity);
		void SetColor(glm::vec3 color);
		
		float GetAmbientIntensity();
		float GetIntensity();
		glm::vec3 GetColor();

		glm::vec3 GetDirectionNormal() const;
		float GetAttenuation(glm::vec3 position) const;

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

	public:
		LightType m_lightType = LightType::UNDEFINED;
		std::variant<directionParams, pointParams, spotParams> m_lightData;
	};

}