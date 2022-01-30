#pragma once;

#include <string>
#include <tuple>
#include <map>
#include <vector>

#include "../../common/include/common.h"

template<class... Data>
class MetadataStorage {
	using DataTypes = std::tuple<Data...>;
	class MetadataID : std::pair<size_t, size_t> {
	 public:
		MetadataID(size_t type_id, size_t data_id)
				: std::pair<size_t, size_t>(type_id, data_id){};
		~MetadataID() = default;
		size_t TypeId() { return first;  }
		size_t DataId() { return second; }
	};
	
 public:
	std::vector<std::string> GetData(size_t shape_id);
	template <typename T, class... Args>
	void AddData(size_t shape_id, T data, const Args... args) {
		shape_id2metadata_id_[shape_id].push_back(AddOnePieceOfData(data));
		AddData(shape_id, args...);
	}
	MetadataStorage() = default;
	~MetadataStorage() = default;
	
 private : 
	template <class T>
	MetadataID AddOnePieceOfData(const T& data);
	std::tuple<std::map<size_t, Data>...> data_;
	std::map<size_t, std::vector<MetadataID>> shape_id2metadata_id_;
};

template <size_t I, typename T, typename Tuple_t>
constexpr size_t index_in_tuple_fn() {
	static_assert(I < std::tuple_size<Tuple_t>::value,
	              "The element is not in the tuple");
	using element = std::tuple_element<I, Tuple_t>::type;
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
MetadataStorage<Data...>::MetadataID
MetadataStorage<Data...>::AddOnePieceOfData(const T& data) {
	constexpr size_t type_id = index_in_tuple<T, DataTypes>::value;
	auto data_storage_of_type = std::get<type_id>(data_);
	const auto needed_element = data_storage_of_type.find(data);
	size_t data_id = UndefinedSizeT;
	if (needed_element == data_storage_of_type.end()) {
		data_id = data_storage_of_type.size();
		data_storage_of_type[data_id] = data;
	} else {
		data_id = needed_element.first;
	}
	return MetadataID(type_id, data_id);
}

template <class... Data>
std::vector<std::string> MetadataStorage<Data...>::GetData(const size_t shape_id) {
	std::vector<std::string> result;
	for (const auto& metadata_id : shape_id2metadata_id_[shape_id]) {
		const auto& data_storage = std::get<metadata_id.TypeId()>(data_);
		// TODO for better formating use fmt or stringstream if needed
		// std::stringstream stream_result;
		// stream_result << data_storage.at(metadata_id.TypeId());
		result.push_back(std::string(data_storage.at(metadata_id.TypeId())));
	}
	return result;
}

