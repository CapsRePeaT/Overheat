#pragma once

#include "idata_provider.h"
#include "virtex_data_classes.h"

class VirtexDataProvider : public IDataProvider {
 public:
	explicit VirtexDataProvider(const VirtexData& data) {}

	GeomStorage<BasicShape> geometry() override { return {}; }
	HeatmapStorage heatmap() override { return {}; }
	MetadataStorage metadata() override { return {}; };
};
