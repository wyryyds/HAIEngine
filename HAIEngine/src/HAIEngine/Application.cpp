#include "Application.h"
#include "hepch.h"

#include "HAIEngine/Log.h"
#include "glad/glad.h"
#include "imgui.h"
#include "Input.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace HAIEngine
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	Application::Application():m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		HE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//------------------------------------渲染三角形部分内容开始线-----------------------------------------
		m_VertexArray.reset(VertexArray::Create());

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

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout m_layout =
		{
			{ShaderDataType::Float3,"a_Position"}
		};
		m_VertexBuffer->SetLayout(m_layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		//--------------------------------------渲染三角形部分结束线----------------------------------

		//--------------------------------------渲染正方形部分开始线----------------------------------
		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" }
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_SquareShader.reset(new Shader(squareVertexSrc, squareFragmentSrc));
		//--------------------------------------渲染正方形部分结束线----------------------------------
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();

	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//输出事件信息
		//HE_CORE_INFO(e.ToString());

		for (auto it = m_LayerStack.end();it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::Run()
	{

		while (m_Running)
		{
			RenderCommand::SetClearColor();
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			//先绘制避免正方形覆盖三角形
			Renderer::Submit(m_SquareShader,m_SquareVA);

			//Renderer::Submit(m_Shader,m_VertexArray);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}
	}

}