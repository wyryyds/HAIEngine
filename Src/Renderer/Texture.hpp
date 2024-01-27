#pragma once
#include "Core/Application.hpp"

#include <string>
#include <memory>

namespace HAIEngine {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(std::string path);
	};

	class Texture3D : public Texture
	{
	public:
		static std::shared_ptr<Texture3D> Create(std::string rtPath, std::string ltPath, std::string ttPath, std::string btPath, std::string backtPath, std::string ftPath);
	};
}