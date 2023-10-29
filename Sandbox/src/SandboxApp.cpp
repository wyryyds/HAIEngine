#include "HAIEngine.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

#include "imgui.h"
#include <glfw/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

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
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		m_SuareVB.reset(HAIEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SuareVB->SetLayout(
			{
				{ HAIEngine::ShaderDataType::Float3, "a_Position" },
				{ HAIEngine::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVA->AddVertexBuffer(m_SuareVB);

		// add test shader
		auto sampleShader = m_ShaderLibrary.Load("sample", "../../../../Sandbox/assets/Shaders/sample.glsl");
		std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(sampleShader)->Bind();

		auto lightingShader = m_ShaderLibrary.Load("lighting", "../../../../Sandbox/assets/Shaders/lighting.glsl");
		std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(lightingShader)->Bind();

		// add shader & texture
		auto textureShader = m_ShaderLibrary.Load("TextureShader", "../../../../Sandbox/assets/Shaders/Texture.glsl");
		m_Texture = HAIEngine::Texture2D::Create("../../../../Sandbox/assets/Textures/d2Texture.png");
		std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(HAIEngine::TimeStep ts) override
	{
		// 输出更新帧率
		//HE_TRACE("update frame time : {0}s {1}ms", ts.GetSeconds(), ts.GetMilliSeconds());
		
		// check inputSystme
		CheckInput(ts);

		// update camera
		m_CameraController->update(ts);

		// rendering
		HAIEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		HAIEngine::RenderCommand::Clear();
		HAIEngine::Renderer::BeginScene(m_PerspectiveCamera);

		auto textureShader = m_ShaderLibrary.Get("TextureShader");
		//m_Texture->Bind();
		// sample shader
		auto sampleShader = std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(m_ShaderLibrary.Get("sample"));
		sampleShader->UploadUniformFloat3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		sampleShader->UploadUniformFloat3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		auto lightingShader =m_ShaderLibrary.Get("lighting");

		HAIEngine::Renderer::Submit(lightingShader, m_SquareVA, glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f)));
		HAIEngine::Renderer::Submit(sampleShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

		HAIEngine::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
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

			m_CameraController->ProcessMouseMovement(curMouseX - lastMouseX, lastMouseY - curMouseY);
			lastMouseX = curMouseX;
			lastMouseY = curMouseY;
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
private:
	HAIEngine::ShaderLibrary m_ShaderLibrary;

	HAIEngine::Camera* m_PerspectiveCamera;
	std::unique_ptr<HAIEngine::CameraController> m_CameraController;

	glm::vec3 m_SquareColor{ 0.1f, 0.2f, 0.3f };
	glm::vec3 lightPos{ 1.2f, 2.0f, -2.0f };

	std::shared_ptr<HAIEngine::VertexBuffer> m_SuareVB;
	std::shared_ptr<HAIEngine::VertexArray> m_SquareVA;

	std::shared_ptr<HAIEngine::Texture2D> m_Texture;

	float lastMouseX, lastMouseY;
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