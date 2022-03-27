#pragma once

#include "idata_provider.h"
#include "type2_TRM.h"

namespace Readers::Type2 {


// FIXME we should return one instance FileRepresentation only
class Type2DataProvider : public IDataProvider {
 public:
  Type2DataProvider(const type2_TRM& geom);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
	HeatmapStorage heatmaps() override { return heatmap_; }
	MetadataStorage metadata() override { return metadata_; };

 private:
	void load_geometry(const type2_TRM& data);


	GeomStorage<BasicShape> geometry_{};
  HeatmapStorage heatmap_{};
	MetadataStorage metadata_{};
};
}  // namespace Readers
