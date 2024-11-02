#pragma once
#include "Core/Core.hpp"
#include "Core/Log.hpp"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

namespace HAIEngine
{
	using json = nlohmann::ordered_json;

	class SerializeHelper
	{
	public:
		SerializeHelper() = default;
		~SerializeHelper() = default;

		static int SerializeData(const int value);
		static std::string SerializeData(const float value);
		static double SerializeData(const double value);
		static size_t SerializeData(const size_t value);
		static std::string SerializeData(std::string_view value);
		static std::string SerializeData(const glm::vec3 value);

		template <typename T>
		static T DeSerializeData(const json& jsonData, std::string_view key)
		{
			if (jsonData.contains(key))
			{
				try
				{
					return jsonData[key].get<T>();
				}
				catch (const json::exception& e)
				{
					LOG_Error("Error reading JSON key!{0}", e.what());
				}
			}

			return T();
		}

		template <typename T>
		static T DeSerializeData(const json& jsonData)
		{
			return jsonData.get<T>();
		}

		template <>
		static float DeSerializeData(const json& jsonData)
		{
			return std::stof(jsonData.get<std::string>());
		}

		template <>
		static glm::vec3 DeSerializeData(const json& jsonData)
		{
			std::istringstream iss(jsonData.get<std::string>());
			std::string token;
			std::vector<float> floatValues;

			while (std::getline(iss, token, ','))
				floatValues.push_back(std::stof(token));

			return glm::vec3{ floatValues[0], floatValues[1], floatValues[2] };
		}

	private:
		static std::string RemoveTrailingZeros(std::string_view str);
	};

	class HESerializeFile
	{
		// TODO add default path
		const std::string DefaultPath = ASSETSPATH;
	public:
		HESerializeFile(std::string_view filepath);
		~HESerializeFile() = default;

		inline json& GetJsonData(){ return m_jsonData; }
		inline void SetJsonData(const json& jsonData) { m_jsonData = jsonData; }
		void Load();
		void Save();

	private:
		std::string m_filepath;
		json m_jsonData;
	};

	class ISerializable
	{
	public:
		ISerializable() : m_guid(GenerateGUID(this)) {}
		virtual ~ISerializable() = default;

		virtual json Serialize() = 0;
		virtual void DeSerialize(const json& jsonData) = 0;

		static size_t GenerateGUID(const ISerializable* obj) {
			return std::hash<const ISerializable*>{}(obj);
		}
		size_t GetGUID() const {
			return m_guid;
		}

	protected:
		size_t m_guid;
	};
}