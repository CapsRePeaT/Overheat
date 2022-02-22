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
		const size_t dummy_layer_id = 0;
		geom_storage.AddShape(std::make_shared<BasicShape>(
				42, dummy_layer_id, Box3D({{0.0f, 10.0f}, {0.0f, 10.0f}, {-0.25f, 0.25f}})));
		geom_storage.AddShape(std::make_shared<BasicShape>(
				43, dummy_layer_id, Box3D({{-15.0f, -8.0f}, {-9.0f, 1.0f}, {0.0f, 0.2f}})));
		geom_storage.AddShape(std::make_shared<BasicShape>(
				44, dummy_layer_id, Box3D({{-3.0f, 8.0f}, {-15.0f, -7.0f}, {0.0f, 0.2f}})));
		return true;
	}
	bool LoadMetadata(const std::string& filename,
	                  MetadataStorage& metadata_storage) {
		std::cout << "metadata loading..." << std::endl;
		return true;
	}
};

void Core::LoadGeometry(const std::string& trm_file,
                        const std::string& t2d_file) {
  // TODO make proper interface class usage
	Readers::VirtexReader reader{trm_file, t2d_file};
	FileRepresentation new_representation;
	new_representation.geom_storage() = reader.geometry();
	new_representation.heatmaps()     = reader.heatmaps();
	representations_.push_back(new_representation);
}
