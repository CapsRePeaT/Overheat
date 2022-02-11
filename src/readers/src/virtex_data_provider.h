#pragma once

#include "T2D.h"
#include "TRM.h"
#include "idata_provider.h"

namespace Readers {


// FIXME we should return one instance FileRepresentation only
class VirtexDataProvider : public IDataProvider {
 public:
	VirtexDataProvider(const TRM& geom, const T2D& heat);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
//	HeatmapStorage heatmap() override { return heatmap_; }
	MetadataStorage metadata() override { return metadata_; };

 private:
	void load_geometry(const TRM& data);
	void load_heatmap(const T2D& data){};
	void load_metadata(){};

	GeomStorage<BasicShape> geometry_{};
	//HeatmapStorage heatmap_{};
	MetadataStorage metadata_{};
};
}  // namespace Readers
