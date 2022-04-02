#pragma once

#include "idata_provider.h"
#include "solver3d_T2D.h"
#include "solver3d_TRM.h"

namespace Readers::Solver3d {


// FIXME we should return one instance FileRepresentation only
class Solver3dDataProvider : public IDataProvider {
 public:
  Solver3dDataProvider(const Solver3d_TRM& geom, const Solver3d_T2D& heat);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
	HeatmapStorage heatmaps() override { return heatmap_; }
	MetadataStorage metadata() override { return metadata_; };

 private:
	void load_geometry(const Solver3d_TRM& data);
	void load_heatmap(const Solver3d_T2D& data);
	void load_metadata(){};

	GeomStorage<BasicShape> geometry_{};
	HeatmapStorage heatmap_{};
	MetadataStorage metadata_{};
};
}  // namespace Readers
