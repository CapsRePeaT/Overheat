#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "../../common/include/common.h"

// TODO: we need data description
// followed structure should be like this:
// data type
// data description
// data array
template <class... Data>
class MetadataStorage {
	using DataTypes = std::tuple<Data...>;
	class MetadataID {
	 public:
		MetadataID(size_t type_id, const std::string& description, size_t data_id)
				: description_(description) {
			ids_[0] = type_id;
			ids_[1] = data_id;
		}
		~MetadataID() = default;
		[[nodiscard]] size_t TypeId() const { return ids_[0]; }
		[[nodiscard]] size_t DataId() const { return ids_[1]; }
		[[nodiscard]] const std::string& Description() const {
			return description_;
		}

	 private:
		// according to Section 23.1.2#8 (associative container requirements) it
		// should work
		const std::string& description_;
		std::array<size_t, 2> ids_{};
	};
	class DataDescriptionAndValue : std::pair<std::string, std::string> {
	 public:
		DataDescriptionAndValue(std::string&& description, std::string&& value)
				: std::pair<std::string, std::string>(description, value) {}
	};

 public:
	std::vector<DataDescriptionAndValue> GetData(size_t shape_id) const;
	template <typename T, class... Args>
	void AddData(size_t shape_id, std::string&& description, T&& data,
	             Args&&... args) {
		shape_id2metadata_id_[shape_id].push_back(
				AddOnePieceOfData(std::forward<T>(data), description));
		AddData(shape_id, std::forward<std::string>(description),
		        std::forward<Args>(args)...);
	}
	MetadataStorage()  = default;
	~MetadataStorage() = default;

 private:
	template <class T>
	MetadataID AddOnePieceOfData(T&& data, std::string&& description);
	std::tuple<std::map<std::string, std::map<size_t, Data>>...> data_;
	std::map<size_t, std::vector<MetadataID>> shape_id2metadata_id_;
};

template <size_t I, typename T, typename Tuple_t>
constexpr size_t index_in_tuple_fn() {
	static_assert(I < std::tuple_size<Tuple_t>::value,
	              "The element is not in the tuple");
	using element = typename std::tuple_element<I, Tuple_t>::type;
	if constexpr (std::is_same<T, element>::value) {
		return I;
	} else {
		return index_in_tuple_fn<I + 1, T, Tuple_t>();
	}
}

template <typename T, typename Tuple_t>
struct index_in_tuple {
	static constexpr size_t value = index_in_tuple_fn<0, T, Tuple_t>();
};

template <class... Data>
template <class T>
typename MetadataStorage<Data...>::MetadataID
MetadataStorage<Data...>::AddOnePieceOfData(T&& data,
                                            std::string&& description) {
	constexpr size_t type_id       = index_in_tuple<T, DataTypes>::value;
	auto data_storages_of_type     = std::get<type_id>(data_);
	const auto& needed_description = data_storages_of_type.find(description);
	size_t data_id                 = 0;
	if (data_storages_of_type[description].find(data) !=
	    data_storages_of_type[description].end())
		data_id = data_storages_of_type[description].find(data).first;
	else
		data_storages_of_type[description][data_id] = data;
	return MetadataID(type_id, data_storages_of_type.find(description).first,
	                  data_id);
}

template <class... Data>
std::vector<typename MetadataStorage<Data...>::DataDescriptionAndValue>
MetadataStorage<Data...>::GetData(const size_t shape_id) const {
	std::vector<std::string> result;
	for (const auto& metadata_id : shape_id2metadata_id_[shape_id]) {
		const auto& data_storage = std::get<metadata_id.TypeId()>(data_);
		// TODO for better formating use fmt or stringstream if needed
		// std::stringstream stream_result;
		// stream_result << data_storage.at(metadata_id.TypeId());
		result.push_back({metadata_id.Description(),
		                  std::string(data_storage.at(metadata_id.Description())
		                                  .at(metadata_id.TypeId()))});
	}
	return result;
}
