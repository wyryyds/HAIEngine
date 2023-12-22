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
	public:
		std::shared_ptr<Mesh> m_mesh;

		virtual json Serialize() override;
		virtual void DeSerialize(const json& jsonData) override;
		virtual void GuiDisplay() override;

	};
}