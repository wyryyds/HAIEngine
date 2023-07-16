#include "HAIEngine.h"

#include "imgui.h"
#include <glfw/glfw3.h>
class ExampleLayer : public HAIEngine::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer") , m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
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

           out vec3 v_Position;

           void main()
           {
               v_Position = a_Position;
               gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new HAIEngine::Shader(vertexSrc, fragmentSrc));
		//--------------------------------------渲染三角形部分结束线----------------------------------

		//--------------------------------------渲染正方形部分开始线----------------------------------
		m_SquareVA.reset(HAIEngine::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<HAIEngine::VertexBuffer> squareVB;
		squareVB.reset(HAIEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
			{
				{ HAIEngine::ShaderDataType::Float3, "a_Position" }
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		std::shared_ptr<HAIEngine::IndexBuffer> squareIB;
		squareIB.reset(HAIEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string squareVertexSrc = R"(
           #version 450 core

           layout(location = 0) in vec3 a_Position;

		   uniform mat4 u_ViewProjection;
           out vec3 v_Position;

           void main()
           {
               v_Position = a_Position;
               gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
           }
        )";

		std::string squareFragmentSrc = R"(
           #version 450 core

           layout(location = 0) out vec4 color;
           
           in vec3 v_Position;
           void main()
           {
               color = vec4(v_Position + 0.5, 1.0);
           }
        )";

		m_SquareShader.reset(new HAIEngine::Shader(squareVertexSrc, squareFragmentSrc));
		//--------------------------------------渲染正方形部分结束线----------------------------------


	}

	void OnUpdate() override
	{
		float time = glfwGetTime();
		HAIEngine::TimeStep timeStep = time - m_LastFrameTime;
		m_LastFrameTime = time;
		HAIEngine::RenderCommand::SetClearColor();
		HAIEngine::RenderCommand::Clear();

		HAIEngine::Renderer::BeginScene(m_Camera);

		//先绘制避免正方形覆盖三角形
		HAIEngine::Renderer::Submit(m_SquareShader, m_SquareVA);

		//Renderer::Submit(m_Shader,m_VertexArray);
	}

	void OnImGuiRender() override
	{
		
		
	}

	void OnEvent(HAIEngine::Event& event) override
	{
	}
private:
	float m_LastFrameTime = 0.0f;

	HAIEngine::OrthographicCamera m_Camera;

	std::shared_ptr<HAIEngine::Shader> m_Shader;
	std::shared_ptr<HAIEngine::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<HAIEngine::VertexArray> m_VertexArray;

	std::shared_ptr<HAIEngine::Shader> m_SquareShader;
	std::shared_ptr<HAIEngine::VertexArray> m_SquareVA;
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