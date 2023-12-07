#include "ImGuiLayer.hpp"
#include "hepch.hpp"
#include "Core/Application.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

#include "imgui.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "ImGuiExamples/imgui_impl_glfw.h"
#include "ImGuiExamples/imgui_impl_opengl3.h"

namespace HAIEngine
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		SetImGuiStyle(); // Set imgui style;

		// Setup Platform/Renderer bindings
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::ImGuiTheme(ImVec3 textColor, ImVec3 headColor, ImVec3 areaColor, ImVec3 bodyColor, ImVec3 popsColor)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		// theem from https://github.com/ocornut/imgui/issues/707#issuecomment-254424199
		style.Colors[ImGuiCol_Text] = ImVec4(textColor.x, textColor.y, textColor.z, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(textColor.x, textColor.y, textColor.z, 0.58f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.95f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.58f);
		style.Colors[ImGuiCol_Border] = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(headColor.x, headColor.y, headColor.z, 0.21f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(headColor.x, headColor.y, headColor.z, 0.80f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(headColor.x, headColor.y, headColor.z, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(headColor.x, headColor.y, headColor.z, 0.15f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(textColor.x, textColor.y, textColor.z, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(textColor.x, textColor.y, textColor.z, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(headColor.x, headColor.y, headColor.z, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(popsColor.x, popsColor.y, popsColor.z, 0.92f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.73f);
	}

	void ImGuiLayer::SetImGuiStyle()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Set imgui front
		ImFontAtlas* fontAtlas = io.Fonts;
		fontAtlas->Clear();
		ImFont* curFont = fontAtlas->AddFontFromFileTTF(ASSETSPATH "Fonts/DroidSans.ttf", 18.0f);
		io.FontDefault = curFont;

		// Setup ImGui componentsColor style
		ImVec3 textColor = ImVec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
		ImVec3 headColor = ImVec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
		ImVec3 areaColor = ImVec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
		ImVec3 bodyColor = ImVec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
		ImVec3 popsColor = ImVec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);
		ImGuiTheme(textColor, headColor, areaColor, bodyColor, popsColor);
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
	}
}

