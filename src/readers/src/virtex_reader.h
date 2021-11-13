#pragma once

#include <optional>

#include "../../core/include/ireader.h"
#include "virtex_data_classes.h"

namespace Readers {
class VirtexReader : public IReader {
 public:
	std::unique_ptr<IDataProvider> load(const fs::path& file) override;
	VirtexData read(const std::string& content);
};
}  // namespace Readers
