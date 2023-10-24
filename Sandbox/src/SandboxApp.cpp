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
		: Layer("ExampleLayer"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_PerspectiveCamera = HAIEngine::CameraController::CreatePerspectiveCamera(HAIEngine::CameraType::PERSPECTIVE,
			1920.0f / 1080.0f, 60.0f, 0.1f, 60.0f);
		m_CameraController = HAIEngine::CameraController::Create(m_PerspectiveCamera);

		//------------------------------------渲染三角形部分内容开始线-----------------------------------------
		m_VertexArray.reset(HAIEngine::VertexArray::Create());

		float vertices[6 * 3] = {
		-0.9f, -0.5f, 0.0f,  // left 
		 0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle 
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f   // top 
		};

		unsigned int indices[] = {
			0, 1, 2, 3, 4, 5
		};

		m_VertexBuffer.reset(HAIEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		HAIEngine::BufferLayout m_layout =
		{
			{HAIEngine::ShaderDataType::Float3,"a_Position"}
		};
		m_VertexBuffer->SetLayout(m_layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		std::shared_ptr<HAIEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(HAIEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
           #version 450 core

           layout(location = 0) in vec3 a_Position;

		   uniform mat4 u_ViewProjection;
		   uniform mat4 u_Transform;

           out vec3 v_Position;

           void main()
           {
               v_Position = a_Position;
               gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
           }
        )";

		std::string fragmentSrc = R"(
           #version 450 core

           layout(location = 0) out vec4 color;

           in vec3 v_Position;

           void main()
           {
               color = vec4(v_Position + 0.5, 1.0);
           }
        )";

		m_Shader = HAIEngine::Shader::Create("m_TestShader", vertexSrc, fragmentSrc);
		//--------------------------------------渲染三角形部分结束线----------------------------------

		//--------------------------------------渲染正方形部分开始线----------------------------------
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


		std::shared_ptr<HAIEngine::VertexBuffer> squareVB;
		squareVB.reset(HAIEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
			{
				{ HAIEngine::ShaderDataType::Float3, "a_Position" },
				{ HAIEngine::ShaderDataType::Float2, "a_TexCoord"}
			});

		m_SquareVA->AddVertexBuffer(squareVB);


		std::string squareVertexSrc = R"(
           #version 450 core

           layout(location = 0) in vec3 a_Position;

		   uniform mat4 u_ViewProjection;
		   uniform mat4 u_Transform;
			
           out vec3 v_Position;

           void main()
           {
               v_Position = a_Position;
               gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
           }
        )";

		std::string squareFragmentSrc = R"(
           #version 450 core

           layout(location = 0) out vec4 color;
           
           in vec3 v_Position;

		   uniform vec3 u_Color;

           void main()
           {
               color = vec4(u_Color, 1.0f);
           }
        )";

		std::shared_ptr<HAIEngine::Shader> tmpShader = 
			HAIEngine::Shader::Create("m_SquareShader", squareVertexSrc, squareFragmentSrc);
		m_SquareShader = std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(tmpShader);

	//--------------------------------------渲染正方形部分结束线----------------------------------

		auto textureShader = m_ShaderLibrary.Load("TextureShader", "../../../../Sandbox/assets/Shaders/Texture.glsl");

		m_Texture = HAIEngine::Texture2D::Create("../../../../Sandbox/assets/Textures/d2Texture.png");
		m_TestTexture = HAIEngine::Texture2D::Create("../../../../Sandbox/assets/Textures/ChernoLogo.png");

		std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<HAIEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(HAIEngine::TimeStep ts) override
	{
		// 输出更新帧率
		//HE_TRACE("update frame time : {0}s {1}ms", ts.GetSeconds(), ts.GetMilliSeconds());

		if (HAIEngine::Input::IsKeyPressed(HE_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (HAIEngine::Input::IsKeyPressed(HE_KEY_A))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		if (HAIEngine::Input::IsKeyPressed(HE_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		HAIEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		HAIEngine::RenderCommand::Clear();

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

		m_CameraController->translate(m_CameraPosition);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		m_CameraController->update(ts);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_PerspectiveCamera->m_projection = projection;
		m_PerspectiveCamera->m_view = view;

		HAIEngine::Renderer::BeginScene(m_PerspectiveCamera);

		m_SquareShader->Bind();
		m_SquareShader->UploadUniformFloat3("u_Color", m_SquareColor);

		auto textureShader = m_ShaderLibrary.Get("TextureShader");
		m_Texture->Bind();
		HAIEngine::Renderer::Submit(textureShader, m_SquareVA);



		/*auto textureShader = m_ShaderLibrary.Get("TextureShader");
		m_Texture->Bind();
		HAIEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		m_TestTexture->Bind();
		HAIEngine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));*/

		// 渲染三角形
		// HAIEngine::Renderer::Submit(m_Shader,m_VertexArray);

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

private:
	HAIEngine::ShaderLibrary m_ShaderLibrary;
	HAIEngine::OrthographicCamera m_Camera;

	std::unique_ptr<HAIEngine::CameraController> m_CameraController;
	HAIEngine::Camera* m_PerspectiveCamera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 20.0f;
	glm::vec3 m_SquarePosition;

	glm::vec3 m_SquareColor = glm::vec3(0.1f, 0.2f, 0.3f);

	std::shared_ptr<HAIEngine::Shader> m_Shader;

	std::shared_ptr<HAIEngine::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<HAIEngine::VertexArray> m_VertexArray;
	std::shared_ptr<HAIEngine::VertexArray> m_SquareVA;

	std::shared_ptr<HAIEngine::Texture2D> m_Texture, m_TestTexture;

	std::shared_ptr<HAIEngine::OpenGLShader> m_SquareShader;
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