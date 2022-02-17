#include "virtex_data_provider.h"

namespace {
Box3D liftBox(const Box3D box, float offset) {
	auto coordinates = box.coordinates();
	coordinates.back().first += offset;
	coordinates.back().second += offset;
	return {coordinates};
}

}  // namespace

namespace Readers {
VirtexDataProvider::VirtexDataProvider(const TRM& geom,
                                       const T2D& heat) {
	load_geometry(geom);
	load_heatmap(heat);
	load_metadata();
}
void VirtexDataProvider::load_geometry(const TRM& data) {
	float offset = 0;
	geometry_.Clear();
	for (const auto& layer : data.layers_) {
		auto geom = layer->geometry();
		for (const auto& shape : geom.get_all_shapes()) {
			shape->setBox(liftBox(shape->bbox(), offset));
			geometry_.AddShape(shape);
		}
		offset += layer->thickness();

	}
}

}  // namespace Readers
