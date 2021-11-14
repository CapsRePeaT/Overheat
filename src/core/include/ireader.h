#pragma once

#include <filesystem>
#include "idata_provider.h"
namespace fs = std::filesystem;

class IReader {
 public:
	GeomStorage<BasicShape*> geometry() { return provider_->geometry(); }
	HeatmapStorage heatmap() { return provider_->heatmap(); }
	MetadataStorage metadata() { return provider_->metadata(); }

 protected:
	virtual void load() = 0;
	std::unique_ptr<IDataProvider> provider_;
	virtual ~IReader() = default;
};
