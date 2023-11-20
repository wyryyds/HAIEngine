#pragma once
#include "HAIEngine/ECS/Component.hpp"

#include <glm/glm.hpp>

namespace HAIEngine
{
	class Transform : public Component
	{
	public:
		const glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3 back = glm::vec3(0.0f, 0.0f, 1.0f);
		const glm::vec3 left = glm::vec3(-1.0f, 0.0f, 0.0f);
		const glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);

		Transform();
		Transform(GameObject* father);
		Transform(glm::vec3 position, GameObject* father);
		~Transform() = default;

		inline glm::vec3 GetPosition() const { return m_position; }
		glm::vec3 GetFront();
		glm::vec3 GetRight();
		glm::vec3 GetUp();
		glm::mat4 GetModelMatrix();

		void translate(glm::vec3 dir, float dis);
		// TODO maybe?
		//virtual void Update() override;
		json Serialize() override;
		void DeSerialize(const json& jsonData) override;

	public:
		glm::vec3 m_position{ 0.0f, 0.0f, 6.0f };
		glm::vec3 m_rotation{ -90.0f, 0.0f, 0.0f};
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
	};
}