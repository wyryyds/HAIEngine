#pragma once
#include "ISerialize.hpp"
#include "hepch.hpp"

namespace HAIEngine
{
	HESerializeFile::HESerializeFile(const std::string& filepath)
		: m_filepath(filepath)
	{
		std::ifstream readfile(m_filepath);
		readfile >> m_jsondata;
		readfile.close();
	}

	void HESerializeFile::Save()
	{
		std::ofstream file(m_filepath);
		file << m_jsondata.dump(4);
		file.close();
	}
}