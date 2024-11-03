#pragma once
#include "Ecs/Component.hpp"
#include "ECS/Mesh.h"


namespace HAIEngine
{
	class OpenGLShader;

	class MeshFilter : public Component
	{
	public:
		MeshFilter() : Component("MeshFilter") { }
		MeshFilter(const std::shared_ptr<Mesh>& mesh) : Component("MeshFilter"), m_mesh(mesh) { }
		MeshFilter(const MeshFilter& other) : Component("MeshFilter"), m_mesh(other.m_mesh) { }
		MeshFilter& operator=(const MeshFilter& other) = default;
		MeshFilter(MeshFilter&& other) noexcept = default;
		MeshFilter& operator=(MeshFilter&& other) noexcept = default;
		virtual ~MeshFilter() override = default;
		virtual std::unique_ptr<Component> Clone() const override { return std::make_unique<MeshFilter>(*this); }

		const std::vector<MeshData>& GetMeshData() const { return m_mesh->GetMeshData(); }

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

		void BindMeshData() const;
		inline void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_mesh = mesh; BindMeshData(); }
	public:
		std::shared_ptr<Mesh> m_mesh;

	};
}