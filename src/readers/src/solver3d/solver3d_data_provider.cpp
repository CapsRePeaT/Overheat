#include "solver3d_data_provider.h"

namespace {
Box3D liftBox(const Box3D box, float offset) {
	auto coordinates = box.coordinates();
	coordinates.back().first += offset;
	coordinates.back().second += offset;
	return {coordinates};
}
}  // namespace

namespace Readers::Solver3d {
Solver3dDataProvider::Solver3dDataProvider(const Solver3d_TRM& geom,
                                           const Solver3d_T2D& heat) {
	load_geometry(geom);
	load_heatmap(heat);
	load_metadata();
}

void Solver3dDataProvider::load_geometry(const Solver3d_TRM& data) {
	float offset          = 0;
	size_t box_counter_   = 0;
	size_t layer_counter_ = 0;
	geometry_.Clear();
	for (const auto& [position, layers] : data.layers_groups_) {
		for (const auto& layer : layers) {
			const auto shapes = layer->geometry().get_all_shapes();
			for (const auto& shape : shapes) {
				geometry_.AddShape(std::make_unique<BasicShape>(
						ShapeId(0, box_counter_++), layer_counter_,
						liftBox(shape->bbox(), offset)));
			}
			++layer_counter_;
			// check that current layer is not Body
			if (!(position == GroupsPosition::Body && layer->type() == LayerType::P))
				offset += layer->thickness();
		}
	}
}

void Solver3dDataProvider::load_heatmap(const Solver3d_T2D& data) {
	heatmap_ = HeatmapStorage(data.net_x(), data.net_y(), data.temperatures());
}

}  // namespace Readers::Solver3d
