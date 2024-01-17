#include "EditorLayer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <iostream>
#include <bitset>

#include "Core/JobSystem.hpp"

namespace HAIEngine
{
	EditorLayer::EditorLayer()
		: Layer("Editor Layer")
	{
		// create camera
		m_PerspectiveCamera = CameraController::CreatePerspectiveCamera(CameraType::PERSPECTIVE,
			1920.0f / 1080.0f, 60.0f, 0.1f, 60.0f);
		m_CameraController = CameraController::Create(m_PerspectiveCamera);

		lastMouseX = Input::GetMouseX();
		lastMouseY = Input::GetMouseY();

		// set vertex data
		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[] =
		{
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};

		m_SuareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SuareVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "aNormal" },
				{ ShaderDataType::Float2, "aTexCoords"}
			});
		m_SquareVA->AddVertexBuffer(m_SuareVB);

		m_LightVA.reset(VertexArray::Create());
		m_LightVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_LightVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "aNormal"},
				{ ShaderDataType::Float2, "aTexCoords"}
			});
		m_LightVA->AddVertexBuffer(m_LightVB);

		m_frameBuffer = FrameBuffer::Create(1920.0f, 1080.0f);
		// add texture
		m_Texture = Texture2D::Create(ASSETSPATH"Textures/container2.png");
		// bind 1 ， 2 to avoid slot conflict
		//m_Texture->Bind(2);
		m_specularTexture = Texture2D::Create(ASSETSPATH"Textures/container2_specular.png");
		//m_specularTexture->Bind(1);

		// add test shader
		auto lightingShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Load("lighting", ASSETSPATH"Shaders/lighting.glsl"));
		auto ModelShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Load("Model", ASSETSPATH"Shaders/Model.glsl"));
		auto sampleColor = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Load("sampleColor", ASSETSPATH"Shaders/sampleColor.glsl"));

		/*auto sampleShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Load("phong", ASSETSPATH"Shaders/phong.glsl"));
		sampleShader->Bind();
		sampleShader->UploadUniformInt("material.diffuse", 2);
		sampleShader->UploadUniformInt("material.specular", 1);*/
		JobSystem::Initialize();
		
		const char* model1Str = ASSETSPATH"/Models/nanosuit/nanosuit.obj";
		const char* model2Str = ASSETSPATH"/Models/kunai/kunai_LOD0.obj";
		{
			auto timer = Timer("Load Models");

			m_meshFilter.SetMesh(std::make_shared<Mesh>(model1Str));
			m_meshRenderer.m_meshFilter = std::make_unique<MeshFilter>(m_meshFilter);

			scene = std::make_shared<Scene>(ASSETSPATH"Jsons/data.json");
		}

		/*std::shared_ptr<HAIEngine::GameObject> testGO1 = std::make_shared<HAIEngine::GameObject>("TestGO1");

		std::shared_ptr<HAIEngine::GameObject> testGO2 = std::make_shared<HAIEngine::GameObject>("TestGO2");

		HAIEngine::Camera* testCamera = new HAIEngine::Camera(HAIEngine::CameraType::PERSPECTIVE,
			1920.0f / 1080.0f, 60.0f, 0.1f, 60.0f);
		testGO1->AddComponent(testCamera);

		scene->SetMainCamera(testCamera);
		scene->AddGameObject(testGO1);
		scene->AddGameObject(testGO2);*/

		scene->Load();

		scene->Save();

		m_panel.SetContexts(scene);
	}

	void EditorLayer::OnAttach()
	{
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		//HE_TRACE("update frame time : {0}s {1}ms", ts.GetSeconds(), ts.GetMilliSeconds());

		// check inputSystme
		CheckInput(ts);

		// update camera
		m_CameraController->update(ts);

		// update gameobjects
		for (int i = 0; i < scene->m_gameObjects.size(); ++i)
		{
			scene->m_gameObjects[i]->Update(ts);
		}
		{
		// rendering
		m_frameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		Renderer::BeginScene(m_PerspectiveCamera->m_projection * m_PerspectiveCamera->m_view);
		//HAIEngine::Renderer::BeginScene(scene->m_mainCamera->GetViewProjection());

		// sample shader
		auto lightingShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("lighting"));
		lightingShader->Bind();
		lightingShader->UploadUniformFloat3("lightColor", m_LightCorlor);
		Renderer::Submit(lightingShader, m_LightVA, glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f)));
		/*auto sampleShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("phong"));
		sampleShader->Bind();
		sampleShader->UploadUniformFloat("material.shininess", m_Specuness);
		sampleShader->UploadUniformFloat3("light.position", lightPos);
		sampleShader->UploadUniformFloat3("viewPos", m_CameraController->GetCameraPosition());
		sampleShader->UploadUniformFloat3("light.ambient", glm::vec3{ 0.2f, 0.2f, 0.2f });
		sampleShader->UploadUniformFloat3("light.specular", m_LightCorlor);
		sampleShader->UploadUniformFloat3("light.diffuse", m_LightCorlor * 0.5f);*/

		auto modelShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("Model"));
		modelShader->Bind();
		modelShader->UploadUniformMat4("u_ViewProjection", m_PerspectiveCamera->m_projection * m_PerspectiveCamera->m_view);
		modelShader->UploadUniformMat4("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		// 1 pass
		RenderCommand::EnableStencilTest();
		RenderCommand::SetStencilFunc(RenderingSetting::EStencilFunc::ALWAYS, 1, 0xFF);

		m_meshRenderer.Draw(modelShader);

		auto modelColorShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("sampleColor"));
		modelColorShader->Bind();
		modelColorShader->UploadUniformMat4("u_ViewProjection", m_PerspectiveCamera->m_projection * m_PerspectiveCamera->m_view);
		modelColorShader->UploadUniformMat4("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		// 2 pass
		RenderCommand::SetStencilFunc(RenderingSetting::EStencilFunc::NOTEQUAL, 1, 0xFF);
		RenderCommand::DisableStencilTest();
		RenderCommand::DisableDepthTest();

		m_meshRenderer.Draw(modelColorShader);

		RenderCommand::SetStencilFunc(RenderingSetting::EStencilFunc::ALWAYS, 1, 0xFF);
		RenderCommand::EnableStencilTest();
		RenderCommand::EnableDepthTest();

		Renderer::EndScene();
		m_frameBuffer->UnBind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		m_panel.OnImguiRender();

		ImGui::Begin("Scene");
		/*ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightCorlor));
		ImGui::ColorEdit3("Cube Color", glm::value_ptr(m_CubeColor));
		ImGui::InputInt("Specular ness", &m_Specuness);
		ImGui::InputFloat3("Light Position", glm::value_ptr(lightPos));*/

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_viewportSize != *((glm::vec2*)&viewportSize))
		{
			m_frameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_viewportSize = { viewportSize.x, viewportSize.y };
		}
		uint32_t textureID = std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_frameBuffer)->GetTextureID();
		ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(textureID)), ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
	}
	void EditorLayer::CheckInput(TimeStep ts)
	{
		if (Input::IsMouseButtonPressed(HE_MOUSE_BUTTON_RIGHT))
		{
			float curMouseX = Input::GetMouseX();
			float curMouseY = Input::GetMouseY();

			if (!IsReControlMouse)
			{
				lastMouseX = curMouseX;
				lastMouseY = curMouseY;
			}

			m_CameraController->ProcessMouseMovement(curMouseX - lastMouseX, lastMouseY - curMouseY);
			lastMouseX = curMouseX;
			lastMouseY = curMouseY;
			IsReControlMouse = true;
		}

		if (Input::IsMouseButtonReleased(HE_MOUSE_BUTTON_RIGHT))
		{
			IsReControlMouse = false;
		}

		if (Input::IsKeyPressed(HE_KEY_LEFT))
		{
			m_CameraController->move(Direction::LEFT, true);
		}
		if (Input::IsKeyPressed(HE_KEY_RIGHT))
		{
			m_CameraController->move(Direction::RIGHT, true);
		}
		if (Input::IsKeyPressed(HE_KEY_UP))
		{
			m_CameraController->move(Direction::UP, true);
		}
		if (Input::IsKeyPressed(HE_KEY_DOWN))
		{
			m_CameraController->move(Direction::DOWN, true);
		}

		if (Input::IsKeyReleased(HE_KEY_LEFT))
		{
			m_CameraController->move(Direction::LEFT, false);
		}
		if (Input::IsKeyReleased(HE_KEY_RIGHT))
		{
			m_CameraController->move(Direction::RIGHT, false);
		}
		if (Input::IsKeyReleased(HE_KEY_UP))
		{
			m_CameraController->move(Direction::UP, false);
		}
		if (Input::IsKeyReleased(HE_KEY_DOWN))
		{
			m_CameraController->move(Direction::DOWN, false);
		}
	}
}