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
#include "FileSystem/AssetsPipeline.hpp"

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
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float3, "a_normal" },
				{ ShaderDataType::Float2, "a_texCoords"}
			});
		m_SquareVA->AddVertexBuffer(m_SuareVB);

		m_LightVA.reset(VertexArray::Create());
		m_LightVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_LightVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float3, "a_normal"},
				{ ShaderDataType::Float2, "a_texCoords"}
			});
		m_LightVA->AddVertexBuffer(m_LightVB);

		float testVertices[] = {
			// positions            // normals         // texcoords
		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	   -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	   -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	   -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};
		m_testVA.reset(VertexArray::Create());
		m_testVB.reset(VertexBuffer::Create(testVertices, sizeof(testVertices)));
		m_testVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_position" },
				{ ShaderDataType::Float3, "a_normal"},
				{ ShaderDataType::Float2, "a_texCoords"}
			});
		m_testVA->AddVertexBuffer(m_testVB);

		// skybox data
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		m_skyboxVA.reset(VertexArray::Create());
		m_skyboxVB.reset(VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices)));
		m_skyboxVB->SetLayout({ {ShaderDataType::Float3, "a_position" } });
		m_skyboxVA->AddVertexBuffer(m_skyboxVB);

		m_MSAAFrameBuffer = FrameBuffer::CreateMSAAFrameBuffer(1920.0f, 1080.0f);
		m_screenFrameBuffer = FrameBuffer::Create(1920.0f, 1080.0f);
		m_depthMap = FrameBuffer::CreateDepthMap();

		AssetsPipeline::LoadAllTextures();
		// add texture
		m_Texture = Texture2D::Create(ASSETSPATH"Textures/window.png");
		m_specularTexture = Texture2D::Create(ASSETSPATH"Textures/container2_specular.png");
		m_skybox = Texture3D::Create(ASSETSPATH"Textures/skybox/right.jpg", ASSETSPATH"Textures/skybox/left.jpg",
			ASSETSPATH"Textures/skybox/top.jpg", ASSETSPATH"Textures/skybox/bottom.jpg",
			ASSETSPATH"Textures/skybox/front.jpg", ASSETSPATH"Textures/skybox/back.jpg");

		// add test shader
		m_ShaderLibrary.Load("lighting", SHADERPATH"lighting.glsl");
		m_ShaderLibrary.Load("Model", SHADERPATH"Model.glsl");
		m_ShaderLibrary.Load("sampleColor", SHADERPATH"sampleColor.glsl");
		m_ShaderLibrary.Load("sampleShader", SHADERPATH"sample.glsl");
		m_ShaderLibrary.Load("skybox", SHADERPATH"skybox.glsl");
		m_ShaderLibrary.Load("shadowMapCaster", SHADERPATH"shadowMapCaster.glsl");
		m_ShaderLibrary.Load("standard", SHADERPATH"standard.glsl");
		
		const char* model1Str = ASSETSPATH"/Models/nanosuit/nanosuit.obj";
		m_meshFilter.SetMesh(std::make_shared<Mesh>(model1Str));

		m_meshRenderer.m_meshFilter = std::make_unique<MeshFilter>(m_meshFilter);

		scene = std::make_shared<Scene>(ASSETSPATH"Jsons/data.json");
		scene->Load();

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

		// rendering
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// render shadowmap
		// test data
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = -100.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		auto shadowMapShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("shadowMapCaster"));
		shadowMapShader->Bind();
		shadowMapShader->UploadUniformMat4("u_lightSpaceMatrix", lightSpaceMatrix);

		m_depthMap->Bind();
		RenderCommand::Clear();
		Renderer::GenerateShadow(shadowMapShader, m_testVA, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 2.0f, 5.0f)), glm::vec3(1.0f)));
		m_meshRenderer.Draw(shadowMapShader);
		m_depthMap->UnBind();

		m_MSAAFrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		Renderer::BeginScene(m_PerspectiveCamera->m_projection * m_PerspectiveCamera->m_view);
		//HAIEngine::Renderer::BeginScene(scene->m_mainCamera->GetViewProjection());

		// skybox
		RenderCommand::SetDepthFunc(RenderingSetting::EDepthFunc::LESS_EQUAL);
		auto skyboxshader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("skybox"));
		skyboxshader->Bind();
		auto view = glm::mat4(glm::mat3(m_PerspectiveCamera->m_view));
		skyboxshader->UploadUniformMat4("u_viewProjection", m_PerspectiveCamera->m_projection * view);
		m_skybox->Bind(0);
		skyboxshader->UploadUniformInt("skybox", 0);
		Renderer::Submit(m_skyboxVA);
		RenderCommand::SetDepthFunc(RenderingSetting::EDepthFunc::LESS);

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
		m_meshRenderer.Draw(modelShader);

		RenderCommand::EnableBlend();
		RenderCommand::SetBlendFunc(RenderingSetting::EBlendFunc::SRC_ALPHA, RenderingSetting::EBlendFunc::ONE_MINUS_SRC_ALPHA);
		auto sampleShader = std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLibrary.Get("sampleShader"));
		m_Texture->Bind(0);
		sampleShader->Bind();
		sampleShader->UploadUniformInt("texturel", 0);
		Renderer::Submit(sampleShader, m_testVA, glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 2.0f, 5.0f)), glm::vec3(1.0f)));
		RenderCommand::DisableBlend();

		RenderCommand::BlitFrameBuffer(m_MSAAFrameBuffer, m_screenFrameBuffer);
		m_MSAAFrameBuffer->UnBind();
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

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_viewportSize != *((glm::vec2*)&viewportSize))
		{
			m_MSAAFrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_screenFrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_viewportSize = { viewportSize.x, viewportSize.y };
		}
		uint32_t textureID = std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_screenFrameBuffer)->GetTextureID();
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