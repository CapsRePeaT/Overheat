#include "solver2d_data_provider.h"

namespace {

void build_layers_shapes(const float width, const float length,
                         const Readers::Solver2d::LayerType& layer,
                         GeomStorage<BasicShape>& storage) {
	auto offset = 0.f;
	for (const auto layer_thickness : layer.zc_.data()) {
		Box3D box{{{0.f, width},
		           {0.f, length},  // need to clarify order
		           {offset, layer_thickness}}};
		storage.AddShape(std::make_unique<BasicShape>(
			               GlobalId(InstanceType::Shape, 0, 0), 0, box));
		offset += layer.position == Readers::Solver2d::Position::Upper
		              ? layer_thickness
		              : -layer_thickness;
	}
}
}  // namespace

namespace Readers::Solver2d {
Solver2dDataProvider::Solver2dDataProvider(const Solver2d_TRM& geom,
                                           const Solver2d_T2D& heat) {
	load_geometry(geom);
	load_heatmap(heat);
}

void Solver2dDataProvider::load_geometry(const Solver2d_TRM& data) {
	geometry_.Clear();
	// read body
	build_layers_shapes(data.size_.width, data.size_.length, data.l1_, geometry_);
	build_layers_shapes(data.size_.width, data.size_.length, data.l2_, geometry_);

	constexpr auto thickness_2d = 0.0f;  // for elements without thickness
	for (const auto& element : data.elements_) {
		const auto coordinates = element.box_;
		Box3D box{{{coordinates.x1_, coordinates.x2_},
		           {coordinates.y1_, coordinates.y2_},
		           {thickness_2d, thickness_2d}}};
		geometry_.AddShape(std::make_unique<BasicShape>(
				GlobalId(InstanceType::Shape, 0, 0), 0, box));
	}
}

void Solver2dDataProvider::load_heatmap(const Solver2d_T2D& heat) {
	heatmap_ = HeatmapStorage(heat.net_x(), heat.net_y(), heat.temperatures(),
	                          heat.design_borders());
}
}  // namespace Readers::Solver2d
