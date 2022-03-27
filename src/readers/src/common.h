#pragma once
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

template <typename T>
class SerializableVec {
 public:
  SerializableVec() = default;

  SerializableVec& set_size(const size_t size) {
    elem_count_ = size;
    data_.reserve(elem_count_);
    return *this;
  }

  friend std::istream& operator>>(std::istream& in, SerializableVec& vec) {
    for (size_t i = 0; i < vec.elem_count_; ++i) {
      T value;
      in >> value;
      vec.data_.push_back(value);
    }
    return in;
  }
  [[nodiscard]] const std::vector<T>& data() const { return data_; }

 private:
  size_t elem_count_ = 0;
  std::vector<T> data_;
};

struct Coordinates {
  float x1_;
  float x2_;
  float y1_;
  float y2_;
};

inline std::string stream_to_string(std::ifstream& stream) {
  return std::string{std::istreambuf_iterator<char>(stream), {}};
}

inline std::string validate_and_get_content(const fs::path& path) {
  std::ifstream ifs{path};
  if (!ifs.good()) {
    throw std::runtime_error("Stream state is not good");
  }
  auto content = stream_to_string(ifs);
  ifs.close();
  return content;
}

struct HorizontalSize {
  float length;
  float width;

  friend std::istream& operator>>(std::istream& in, HorizontalSize& obj) {
    in >> obj.length >> obj.width;  // mb this order is wrong. TBD with Ryabov
    return in;
  }
};