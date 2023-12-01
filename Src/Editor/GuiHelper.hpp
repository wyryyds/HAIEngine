#pragma once
#include "ECS/Components/Camera.hpp"

namespace HAIEngine
{
	class GuiHelper
	{
	public:
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

		static void DrawEnumControl(CameraType& type);
	private:
		static const char* CameraTypeString[];
		static inline const char* CameraType2String(CameraType type) { return CameraTypeString[static_cast<int>(type)]; }
		static CameraType String2CameraType(const char* typeString);
	};
}