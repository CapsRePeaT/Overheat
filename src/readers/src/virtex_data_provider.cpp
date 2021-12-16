#include "virtex_data_provider.h"

namespace {
Box3D liftBox(Box3D box, float offset) {
	auto coordinates = box.coordinates();
  coordinates.back().first += offset;
  coordinates.back().second += offset;
	return {coordinates};
}

}  // namespace

namespace Readers {
VirtexDataProvider::VirtexDataProvider(const VirtexData& data) {
	load_geometry(data);
	load_heatmap(data);
	load_metadata(data);
}
void VirtexDataProvider::load_geometry(const VirtexData& data) {
	float offset = 0;
	geometry_.Clear();
	for (const auto& layer : data.layers_) {
		auto geom = layer->getGeometry();
		for (const auto& shape : geom.get_all_shapes()) {
			shape->setBox(liftBox(shape->bbox(), offset));
			offset += layer->getThickness();
			geometry_.AddShape(shape);
		}
	}
}

}  // namespace Readers