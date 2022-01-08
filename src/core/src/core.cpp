#include "core.h"
#include <iostream>
#include "../../readers/src/virtex_reader.h"

// TODO delete after proper reader implementation (or leave for testing, TBD)
// TODO 2 add proper logging
class DummyReader {
 public:
	bool LoadGeometry(const std::string& filename,
	                  GeomStorage<BasicShape>& geom_storage) {
		std::cout << "file to load shapes from:" << filename << std::endl;
		geom_storage.AddShape(std::make_shared<BasicShape>(
				42, Box3D({{0.0f, 10.0f}, {0.0f, 10.0f}, {-0.25f, 0.25f}})));
		geom_storage.AddShape(std::make_shared<BasicShape>(
				43, Box3D({{-15.0f, -8.0f}, {-9.0f, 1.0f}, {0.0f, 0.2f}})));
		geom_storage.AddShape(std::make_shared<BasicShape>(
				44, Box3D({{-3.0f, 8.0f}, {-15.0f, -7.0f}, {0.0f, 0.2f}})));
		return true;
	}
	// Сan we use std::ostream instead?
	bool LoadHeatmap(const std::string& filename,
	                 HeatmapStorage& heatmap_storage) {
		std::cout << "heatmap loading..." << std::endl;
		return true;
	}
	bool LoadMetadata(const std::string& filename,
	                  MetadataStorage& metadata_storage) {
		std::cout << "metadata loading..." << std::endl;
		return true;
	}
};


Core::Shapes Core::GetShapes(const Box3D& area) {
  return geom_storage_.get_all_shapes();
}

void Core::LoadGeometry(const std::string& file_name) {
  // TODO make proper interface class usage
	Readers::VirtexReader reader{file_name};
	geom_storage_ = reader.geometry();
}
