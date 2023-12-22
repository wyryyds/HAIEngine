#pragma once
#include "ECS/Component.hpp"

namespace HAIEngine
{
	class MeshFilter;
	class OpenGLShader;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer() : Component("MeshRenderer") {}
		~MeshRenderer() = default;

		void Draw(std::shared_ptr<OpenGLShader> shader);

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

		std::unique_ptr<MeshFilter> m_meshFilter;
	};

}