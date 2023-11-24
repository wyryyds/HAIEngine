#include "HAIEngine.hpp"
#include "Core/EntryPoint.hpp"
#include "Editor/EditorLayer.hpp"

namespace HAIEngine
{
	class Editor : public Application
	{
	public:
		Editor() : Application("HAIEngine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Editor() = default;
	};

	Application* CreateApplication()
	{
		return new Editor();
	}
}