#include "Core/Core.hpp"
#include "Core/ISerialize.hpp"
#include "HAIEngine.hpp"
#include "Core/Reflection.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include <glfw/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

//test
#include "ECS/Components/Camera.hpp"
#include "ECS/GameObject.hpp"
#include "ECS/Scene.hpp"

using json = nlohmann::ordered_json;

class ExampleLayer : public HAIEngine::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		// create camera
		m_PerspectiveCamera = HAIEngine::CameraController::CreatePerspectiveCamera(HAIEngine::CameraType::PERSPECTIVE,
			1920.0f / 1080.0f, 60.0f, 0.1f, 60.0f);
		m_CameraController = HAIEngine::CameraController::Create(m_PerspectiveCamera);

		lastMouseX = HAIEngine::Input::GetMouseX();
		lastMouseY = HAIEngine::Input::GetMouseY();

		// set vertex data
		m_SquareVA.reset(HAIEngine::VertexArray::Create());

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

		m_SuareVB.reset(HAIEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SuareVB->SetLayout(
			{
				{ HAIEngine::ShaderDataType::Float3, "a_Position" },
				{ HAIEngine::ShaderDataType::Float3, "aNormal" },
				{ HAIEngine::ShaderDataType::Float2, "aTexCoords"}
			});
		m_SquareVA->AddVertexBuffer(m_SuareVB);

		m_LightVA.reset(HAIEngine::VertexArray::Create());
		m_LightVB.reset(HAIEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_LightVB->SetLayout(
			{
				{ HAIEngine::ShaderDataType::Float3, "a_Position" },
				{ HAIEngine::ShaderDataType::Float3, "aNormal"},
				{HAIEngine::ShaderDataType::Float2, "aTexCoords"}
			});
		m_LightVA->AddVertexBuffer(m_LightVB);

		m_frameBuffer = HAIEngine::FrameBuffer::Create(1920.0f, 1080.0f);
		// add texture
		m_Texture = HAIEngine::Texture2D::Create("../../../../Sandbox/assets/Textures/container2.png");
		m_Texture->Bind(0);
		m_specularTexture = HAIEngine::Texture2D::Create("../../../../Sandbox/assets/Textures/container2_specular.png");
		m_specularTexture->Bind(1);

		// add test shader
		auto lightingShader = std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(m_ShaderLibrary.Load("lighting", "../../../../Sandbox/assets/Shaders/lighting.glsl"));

		auto sampleShader = std::dynamic_pointer_cast<HAIEngine::OpenGLShader>( m_ShaderLibrary.Load("phong", "../../../../Sandbox/assets/Shaders/phong.glsl"));
		sampleShader->Bind();
		sampleShader->UploadUniformInt("material.diffuse", 0);
		sampleShader->UploadUniformInt("material.specular", 1);

		//test json
		//HAIEngine::Scene myScene(ASSTESPATH"Jsons/data.json");
		
		scene = std::make_shared<HAIEngine::Scene>(ASSTESPATH"Jsons/data.json");

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
	}

	void OnUpdate(HAIEngine::TimeStep ts) override
	{
		// 输出更新帧率
		//HE_TRACE("update frame time : {0}s {1}ms", ts.GetSeconds(), ts.GetMilliSeconds());
		
		// check inputSystme
		CheckInput(ts);

		// update camera
		m_CameraController->update(ts);

		m_frameBuffer->Bind();
		// rendering
		HAIEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		HAIEngine::RenderCommand::Clear();

		for (int i = 0; i < scene->m_gameObjects.size(); ++i)
		{
			scene->m_gameObjects[i]->Update(ts);
		}

		HAIEngine::Renderer::BeginScene(m_PerspectiveCamera->m_projection * m_PerspectiveCamera->m_view);
		//HAIEngine::Renderer::BeginScene(scene->m_mainCamera->GetViewProjection());

		// sample shader
		auto lightingShader = std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(m_ShaderLibrary.Get("lighting"));
		lightingShader->Bind();
		lightingShader->UploadUniformFloat3("lightColor", m_LightCorlor);

		auto sampleShader = std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(m_ShaderLibrary.Get("phong"));
		sampleShader->Bind();
		sampleShader->UploadUniformFloat("material.shininess", m_Specuness);
		sampleShader->UploadUniformFloat3("light.position", lightPos);
		sampleShader->UploadUniformFloat3("viewPos", m_CameraController->GetCameraPosition());
		sampleShader->UploadUniformFloat3("light.ambient", glm::vec3{ 0.2f, 0.2f, 0.2f });
		sampleShader->UploadUniformFloat3("light.specular", m_LightCorlor);
		sampleShader->UploadUniformFloat3("light.diffuse", m_LightCorlor * 0.5f);

		HAIEngine::Renderer::Submit(lightingShader, m_LightVA, glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f)));

		HAIEngine::Renderer::Submit(sampleShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
		HAIEngine::Renderer::Submit(sampleShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

		HAIEngine::Renderer::EndScene();
		m_frameBuffer->UnBind();
		HAIEngine::RenderCommand::Clear();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightCorlor));
		ImGui::ColorEdit3("Cube Color", glm::value_ptr(m_CubeColor));
		ImGui::InputInt("Specular ness", &m_Specuness);
		ImGui::InputFloat3("Light Position", glm::value_ptr(lightPos));
		ImGui::Text("Scene:");
		uint32_t textureID = std::dynamic_pointer_cast<HAIEngine::OpenGLFrameBuffer>(m_frameBuffer)->GetTextureID();
		ImGui::Image((void*)textureID, ImVec2{ 1920, 1080 }, ImVec2{ 0, 1 }, ImVec2{1, 0});
		ImGui::Text("End Scene");
		ImGui::End();
	}

	void OnEvent(HAIEngine::Event& event) override
	{
	}

	void CheckInput(HAIEngine::TimeStep ts)
	{
		if (HAIEngine::Input::IsMouseButtonPressed(HE_MOUSE_BUTTON_RIGHT))
		{
			float curMouseX = HAIEngine::Input::GetMouseX();
			float curMouseY = HAIEngine::Input::GetMouseY();

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

		if (HAIEngine::Input::IsMouseButtonReleased(HE_MOUSE_BUTTON_RIGHT))
		{
			IsReControlMouse = false;
		}

		if (HAIEngine::Input::IsKeyPressed(HE_KEY_LEFT))
		{
			m_CameraController->move(HAIEngine::Direction::LEFT, true);
		}
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_RIGHT))
		{
			m_CameraController->move(HAIEngine::Direction::RIGHT, true);
		}
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_UP))
		{
			m_CameraController->move(HAIEngine::Direction::UP, true);
		}
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_DOWN))
		{
			m_CameraController->move(HAIEngine::Direction::DOWN, true);
		}

		if (HAIEngine::Input::IsKeyReleased(HE_KEY_LEFT))
		{
			m_CameraController->move(HAIEngine::Direction::LEFT, false);
		}
		if (HAIEngine::Input::IsKeyReleased(HE_KEY_RIGHT))
		{
			m_CameraController->move(HAIEngine::Direction::RIGHT, false);
		}
		if (HAIEngine::Input::IsKeyReleased(HE_KEY_UP))
		{
			m_CameraController->move(HAIEngine::Direction::UP, false);
		}
		if (HAIEngine::Input::IsKeyReleased(HE_KEY_DOWN))
		{
			m_CameraController->move(HAIEngine::Direction::DOWN, false);
		}

		if (HAIEngine::Input::IsKeyPressed(HE_KEY_A))
		{
		}
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_D))
		{
		}

	}

	void TestCode()
	{

	}

	void AddReflection()
	{
	}
private:
	HAIEngine::ShaderLibrary m_ShaderLibrary;

	HAIEngine::EditorCamera* m_PerspectiveCamera;
	std::unique_ptr<HAIEngine::CameraController> m_CameraController;

	glm::vec3 m_LightCorlor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_CubeColor{ 1.0f, 0.5f, 0.31f };
	glm::vec3 lightPos{ 1.2f, 2.0f, 2.0f };
	int m_Specuness = 64;

	std::shared_ptr<HAIEngine::VertexBuffer> m_SuareVB, m_LightVB;
	std::shared_ptr<HAIEngine::VertexArray> m_SquareVA, m_LightVA;

	std::shared_ptr<HAIEngine::Texture2D> m_Texture, m_specularTexture;

	std::shared_ptr<HAIEngine::FrameBuffer> m_frameBuffer;

	float lastMouseX, lastMouseY;
	bool IsReControlMouse = false;

	//sample ECS
	std::shared_ptr<HAIEngine::Scene> scene;
};

class Sandbox : public HAIEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{	
	}

};

HAIEngine::Application* HAIEngine::CreateApplication()
{
	return new Sandbox();
}