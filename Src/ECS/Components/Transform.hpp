#pragma once
#include "ECS/Component.hpp"

#include <glm/glm.hpp>

namespace HAIEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		Transform(GameObject* father);
		Transform(glm::vec3 position, GameObject* father);
		Transform(const Transform& other) = default;
		Transform& operator=(const Transform& other) = default;
		Transform(Transform&& other) = default;
		Transform& operator=(Transform&& other) = default;
		virtual ~Transform() override = default;
		virtual std::unique_ptr<Component> Clone() const override { return std::make_unique<Transform>(*this); }
		
		inline glm::vec3 GetPosition() const { return m_position; }
		glm::vec3 GetFront() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;
		glm::mat4 GetModelMatrix() const;

		void translate(glm::vec3 dir, float dis);
		// TODO maybe?
		//virtual void Update() override;
		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

	public:
		glm::vec3 m_position{ 0.0f, 0.0f, 6.0f };
		glm::vec3 m_rotation{ -90.0f, 0.0f, 0.0f};
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

	private:
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 back = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 left = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
	};
}