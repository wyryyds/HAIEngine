#pragma once
#include "ECS/Component.hpp"

namespace HAIEngine
{
	class MeshFilter;
	class OpenGLShader;
	class FrameBuffer;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer() : Component("MeshRenderer") {}
		MeshRenderer(const MeshRenderer& other);
		MeshRenderer(MeshRenderer&& other) noexcept = default;
		MeshRenderer& operator=(const MeshRenderer& other);
		MeshRenderer& operator=(MeshRenderer&& other) noexcept = default;
		~MeshRenderer() override = default;
		virtual std::unique_ptr<Component> Clone() const override { return std::make_unique<MeshRenderer>(*this); }

		void Draw(const std::shared_ptr<OpenGLShader>& shader, const std::shared_ptr<FrameBuffer>& depthMap) const;

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

		std::unique_ptr<MeshFilter> m_meshFilter;
	};

}