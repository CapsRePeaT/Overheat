#pragma once

#include "idata_provider.h"
#include "solver2d_TRM.h"

namespace Readers::Solver2d {

// FIXME we should return one instance FileRepresentation only
class Solver2dDataProvider : public IDataProvider {
 public:
	explicit Solver2dDataProvider(const Solver2d_TRM& geom);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
	HeatmapStorage heatmaps() override { return heatmap_; }
	MetadataStorage metadata() override { return metadata_; };

 private:
	void load_geometry(const Solver2d_TRM& data);

	GeomStorage<BasicShape> geometry_{};
	HeatmapStorage heatmap_{};
	MetadataStorage metadata_{};
};
}  // namespace Readers::Solver2d
