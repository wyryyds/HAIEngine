#pragma once
#include "Core/Core.hpp"
#include "Core/Log.hpp"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

namespace HAIEngine
{
	using json = nlohmann::ordered_json;

	namespace JsonSerializeHelper
	{
		template <typename T>
		T DeSerializeData(const json& jsonData, std::string_view key)
		{
			if (jsonData.contains(key))
				return jsonData[key].get<T>();

			return T();
		}

		template <typename T>
		T DeSerializeData(const json& jsonData)
		{
			return jsonData.get<T>();
		}

		template <>
		inline float DeSerializeData(const json& jsonData)
		{
			return std::stof(jsonData.get<std::string>());
		}

		template <>
		inline glm::vec3 DeSerializeData(const json& jsonData)
		{
			std::istringstream iss(jsonData.get<std::string>());
			std::string token;
			std::vector<float> floatValues;

			while (std::getline(iss, token, ','))
				floatValues.push_back(std::stof(token));

			return glm::vec3{ floatValues[0], floatValues[1], floatValues[2] };
		}
	}

	class SerializeHelper
	{
	public:
		static int SerializeData(const int value);
		static std::string SerializeData(const float value);
		static double SerializeData(const double value);
		static size_t SerializeData(const size_t value);
		static std::string SerializeData(std::string_view value);
		static std::string SerializeData(const glm::vec3 value);

	private:
		static std::string RemoveTrailingZeros(std::string_view str);
	};

	class HESerializeFile
	{
		// TODO add default path
		std::string DefaultPath = ASSETSPATH;
	public:
		HESerializeFile(std::string_view filepath);
		HESerializeFile(const HESerializeFile& other) = default;
		HESerializeFile& operator=(const HESerializeFile& other) = default;
		HESerializeFile(HESerializeFile&& other) = default;
		HESerializeFile& operator=(HESerializeFile&& other) = default;
		~HESerializeFile() = default;

		inline json& GetJsonData(){ return m_jsonData; }
		inline void SetJsonData(const json& jsonData) { m_jsonData = jsonData; }
		void Load();
		void Save() const;

	private:
		std::string m_filepath;
		json m_jsonData;
	};

	class ISerializable
	{
	public:
		ISerializable() : m_guid(GenerateGUID(this)) {}
		ISerializable(const ISerializable& other) = default;
		ISerializable(ISerializable&& other) = default;
		ISerializable& operator=(const ISerializable& other) = default;
		ISerializable& operator=(ISerializable&& other) = default;
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