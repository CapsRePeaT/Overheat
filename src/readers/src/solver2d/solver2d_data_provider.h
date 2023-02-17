#pragma once

#include "idata_provider.h"
#include "solver2d_T2D.h"
#include "solver2d_TRM.h"

namespace Readers::Solver2d {

// FIXME we should return one instance FileRepresentation only
class Solver2dDataProvider : public IDataProvider {
 public:
	explicit Solver2dDataProvider(const Solver2d_TRM& geom,
	                              const Solver2d_T2D& heat);

	GeomStorage<BasicShape> geometry() override { return geometry_; };
	HeatmapStorage heatmaps() override { return heatmap_; }
	DefaultMetadataStorage metadata() override { return metadata_; };
	LayersShapes layers_shapes() override { return {}; };
	ShapesHeatData shapes_metadata()  override { return {}; };

			private:
	void load_geometry(const Solver2d_TRM& data);
	void load_heatmap(const Solver2d_T2D& heat, float env_temp);

	GeomStorage<BasicShape> geometry_{};
	HeatmapStorage heatmap_{};
	DefaultMetadataStorage metadata_{};
};
}  // namespace Readers::Solver2d
