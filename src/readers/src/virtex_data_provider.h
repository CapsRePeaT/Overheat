#pragma once

#include "idata_provider.h"
#include "virtex_data_classes.h"

namespace Readers {


// FIXME we should return one instance FileRepresentation only
class VirtexDataProvider : public IDataProvider {
 public:
	explicit VirtexDataProvider(const VirtexData& data);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
//	HeatmapStorage heatmap() override { return heatmap_; }
	MetadataStorage metadata() override { return metadata_; };

 private:
	void load_geometry(const VirtexData& data);
	void load_heatmap(const VirtexData& data){};
	void load_metadata(const VirtexData& data){};

	GeomStorage<BasicShape> geometry_{};
	//HeatmapStorage heatmap_{};
	MetadataStorage metadata_{};
};
}  // namespace Readers
