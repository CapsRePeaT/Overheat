#pragma once

#include <filesystem>
#include "idata_provider.h"
namespace fs = std::filesystem;

class IReader {
 public:
	GeomStorage<BasicShape*> geometry() { return data_provider_->geometry(); }
	HeatmapStorage heatmap() { return data_provider_->heatmap(); }
	MetadataStorage metadata() { return data_provider_->metadata(); }

 protected:
	virtual void load() = 0;
	std::unique_ptr<IDataProvider> data_provider_;
	virtual ~IReader() = default;
};
