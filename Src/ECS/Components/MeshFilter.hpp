#pragma once
#include "Ecs/Component.hpp"
#include "ECS/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace HAIEngine
{
	class OpenGLShader;

	class MeshFilter : public Component
	{
	public:
		MeshFilter() : Component("MeshFilter") { }
		MeshFilter(std::shared_ptr<Mesh> mesh) : Component("MeshFilter"), m_mesh(mesh) { }

		const std::vector<MeshData>& GetMeshDatas() const { return m_mesh->GetMeshData(); }

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

		void BindMeshData() const;
		inline void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; BindMeshData(); }
	public:
		std::shared_ptr<Mesh> m_mesh;

	};
}