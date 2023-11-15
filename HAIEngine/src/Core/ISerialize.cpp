#pragma once
#include "ISerialize.hpp"
#include "hepch.hpp"

namespace HAIEngine
{
	HESerializeFile::HESerializeFile(const std::string& filepath)
		: m_filepath(filepath)
	{
	}

	void HESerializeFile::Load()
	{
		std::ifstream readfile(m_filepath);
		readfile >> m_jsonData;
		readfile.close();
	}

	void HESerializeFile::Save()
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
		return std::to_string(value);
	}
	double SerializeHelper::SerializeData(const double value)
	{
		return value;
	}
	size_t SerializeHelper::SerializeData(const size_t value)
	{
		return value;
	}
	std::string SerializeHelper::SerializeData(const std::string& value)
	{
		return value;
	}
	std::string SerializeHelper::SerializeData(const glm::vec3 value)
	{
		std::stringstream ss;
		ss << value.x << "," << value.y << "," << value.z;

		return ss.str();
	}
}