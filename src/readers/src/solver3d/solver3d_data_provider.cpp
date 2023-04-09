#include "solver3d_data_provider.h"

namespace {
Box3D liftBox(const Box3D& box, float offset) {
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
	ambient_temperature_ = data.metadata_.env_temperature;
	geometry_.Clear();

	for (const auto& [position, layers] : data.layers_groups_) {
		for (const auto& layer : layers) {
			std::vector<BasicShape> layer_shapes;
			const auto shapes_data = layer->shape_data();
			for (const auto& [heat_data, shape] : shapes_data) {
				auto box = liftBox(shape.bbox(), offset);

				const auto global_id = GlobalId(InstanceType::Shape, box_counter_++, 0);
				geometry_.AddShape(
						std::make_unique<BasicShape>(global_id, layer_counter_, box));
				layer_shapes.emplace_back(global_id, layer_counter_, box);

				shapes_metadata_.emplace_back(heat_data);

			}
			layers_shapes_.push_back(layer_shapes);
			++layer_counter_;
			// check that current layer is not Body
			if (!(position == GroupsPosition::Body && layer->type() == LayerType::P))
				offset += layer->thickness();
		}
	}
}

void Solver3dDataProvider::load_heatmap(const Solver3d_T2D& data) {
	if (data.temperatures().size())
		heatmap_ = HeatmapStorage(data.net_x(), data.net_y(), data.temperatures(),
	                          data.env_temp(), data.design_borders());
}

}  // namespace Readers::Solver3d
