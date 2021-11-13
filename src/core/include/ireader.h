#pragma once

#include <filesystem>

#include "idata_provider.h"

namespace fs = std::filesystem;

class IReader {
 public:
	// TODO: we need to identify is it possible to detect geometry, heatmap and
	// metadata file by some single file. If it is not possible we need to
	// consider the option of adding such data to output file
	virtual std::unique_ptr<IDataProvider> load(const fs::path& file) = 0;

 protected:
	virtual ~IReader() = default;
};
