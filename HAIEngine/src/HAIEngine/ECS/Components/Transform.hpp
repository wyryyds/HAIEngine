#pragma once
#include "HAIEngine/ECS/Component.hpp"
#include "HAIEngine/ECS/GameObject.hpp"

#include <glm/glm.hpp>

namespace HAIEngine
{
	class GameObject;

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
		Transform(glm::vec3 position);
		~Transform() = default;

		glm::vec3 GetFront();
		glm::vec3 GetRight();
		glm::vec3 GetUp();
		glm::mat4 GetModelMatrix();

		void translate(glm::vec3 dir, float dis);
		// TODO maybe?
		//virtual void Update() override;
		virtual json Serialize(const std::string& name) override;
		virtual void DeSerialize(const json& jsondata) override;
	public:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		GameObject* m_parent;
	};
}