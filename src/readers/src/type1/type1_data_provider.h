#pragma once

#include "idata_provider.h"
#include "type1_T2D.h"
#include "type1_TRM.h"

namespace Readers::Type1 {


// FIXME we should return one instance FileRepresentation only
class Type1DataProvider : public IDataProvider {
 public:
  Type1DataProvider(const TRM& geom, const type1_T2D& heat);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
	HeatmapStorage heatmaps() override { return heatmap_; }
	MetadataStorage metadata() override { return metadata_; };

 private:
	void load_geometry(const TRM& data);
	void load_heatmap(const type1_T2D& data);
	void load_metadata(){};

	GeomStorage<BasicShape> geometry_{};
	HeatmapStorage heatmap_{};
	MetadataStorage metadata_{};
};
}  // namespace Readers
