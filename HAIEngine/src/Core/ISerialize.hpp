#pragma once
#include "Core/Core.hpp"

#include "nlohmann/json.hpp"

namespace HAIEngine
{
	using json = nlohmann::ordered_json;

	class HESerializeFile
	{
		// TODO add default path
		const std::string DefaultPath = ASSTESPATH;
	public:
		HESerializeFile(const std::string& filepath);
		~HESerializeFile() = default;

		inline json& GetJsonData(){ return m_jsonData; }
		inline void SetJsonData(const json& jsondata) { m_jsonData = jsondata; }
		void Load();
		void Save();

	private:
		std::string m_filepath;
		json m_jsonData;
	};

	class ISerializeable
	{
	public:
		ISerializeable() : m_guid(GenerateGUID(this)) {}
		virtual ~ISerializeable() = default;

		virtual json Serialize(const std::string& name) = 0;
		virtual void DeSerialize(const json& jsondata) = 0;

		static size_t GenerateGUID(const ISerializeable* obj) {
			return std::hash<const ISerializeable*>{}(obj);
		}
		size_t GetGUID() const {
			return m_guid;
		}

	protected:
		size_t m_guid;
	};
}