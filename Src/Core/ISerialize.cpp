#pragma once
#include "ISerialize.hpp"
#include "hepch.hpp"

namespace HAIEngine
{
	HESerializeFile::HESerializeFile(std::string_view filepath)
		: m_filepath(filepath)
	{
	}

	void HESerializeFile::Load()
	{
		std::ifstream readFile(m_filepath);
		readFile >> m_jsonData;
		readFile.close();
	}

	void HESerializeFile::Save() const
	{
		std::ofstream file(m_filepath);
		file << m_jsonData.dump(2);
		file.close();
	}

	int SerializeHelper::SerializeData(const int value)
	{
		return value;
	}

	std::string SerializeHelper::SerializeData(const float value)
	{
		return  RemoveTrailingZeros(std::to_string(value));
	}

	double SerializeHelper::SerializeData(const double value)
	{
		return value;
	}

	size_t SerializeHelper::SerializeData(const size_t value)
	{
		return value;
	}

	std::string SerializeHelper::SerializeData(std::string_view value)
	{
		return value.data();
	}

	std::string SerializeHelper::SerializeData(const glm::vec3 value)
	{
		std::stringstream ss;
		ss << value.x << "," << value.y << "," << value.z;

		return ss.str();
	}
	
	std::string SerializeHelper::RemoveTrailingZeros(std::string_view str) {
		size_t dotPos = str.find_last_not_of('0');
		if (dotPos != std::string_view::npos && str[dotPos] == '.') {
			return std::string(str.substr(0, dotPos));
		}
		return std::string(str.substr(0, dotPos + 1));
	}
}