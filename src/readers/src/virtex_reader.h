#pragma once

#include <filesystem>
#include <optional>

#include "../../core/include/ireader.h"
#include "virtex_data_classes.h"

namespace fs = std::filesystem;

namespace Readers {
VirtexData read(const std::string& content);

// Reader implementation for files similar to described in doc/virtex.txt
class VirtexReader : public IReader {
 public:
	explicit VirtexReader(const fs::path& file) : file_(file) { load(); };

 protected:
	void load() override;

 private:
	fs::path file_;
};
}  // namespace Readers
