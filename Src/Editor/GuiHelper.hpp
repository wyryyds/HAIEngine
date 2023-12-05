#pragma once
#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Light.hpp"

namespace HAIEngine
{
	class GuiHelper
	{
	public:
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

		static void DrawFloatSlider(const char* label, float* v, float v_min, float v_max);
		static void DrawFloatInput(const char* label, float* v);
		static void DrawEnumControl(CameraType& type);
		static void DrawEnumControl(LightType& type);
	private:
		static const char* CameraTypeString[];
		static inline const char* CameraType2String(CameraType type) { return CameraTypeString[static_cast<int>(type)]; }
		static CameraType String2CameraType(const char* typeString);

		static const char* LightTypeString[];
		static inline const char* LightType2String(LightType type) { return LightTypeString[static_cast<int>(type)]; }
		static LightType String2LightType(const char* typeString);

		template <typename T>
		static T String2EnumValue(const char* typeString, const char* enumStrings[], int enumCount)
		{
			for (int i = 0; i < enumCount; ++i) {
				if (strcmp(typeString, enumStrings[i]) == 0) {
					return static_cast<T>(i);
				}
			}
			return static_cast<T>(0);
		}
	};
}