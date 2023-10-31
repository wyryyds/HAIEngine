#pragma once
#include "HAIEngine/Core.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace HAIEngine
{
	class HESerializeFile
	{
		// TODO add default path
		const std::string DefaultPath = ASSTESPATH;
	public:
		HESerializeFile(const std::string& filepath);
		~HESerializeFile() = default;
		inline json& GetJsonData(){ return m_jsondata; }
		void Save();

	private:
		std::string m_filepath;
		json m_jsondata;
	};
}