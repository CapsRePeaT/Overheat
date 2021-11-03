#pragma once

#include <string>
#include <iostream>
#include <array>

#include "../src/databases.h"
#include "../src/shapes.h"
#include "../src/heatmap.h"

class IReader {
 public:
  // TODO: we need to identify is it possible to detect geometry, heatmap and metadata file by some single file.
  // If it is not possible we need to consider the option of adding such data to output file
  virtual bool LoadGeometry(const std::string& filename, GeomStorage<BasicShape>& geom_storage) = 0;
  // Сan we use std::ostream instead?
  virtual bool LoadHeatmap(const std::string& filename, HeatmapStorage& heatmap_storage) = 0;
  virtual bool LoadMetadata(const std::string& filename, MetadataStorage& metadata_storage) = 0;
 protected:
  virtual ~IReader() = default;
};

// TODO delete after proper reader implementation (or leave for testing, TBD)
// TODO 2 add proper logging
class DummyReader : IReader {
 public:
  bool LoadGeometry(const std::string& filename,
                    GeomStorage<BasicShape>& geom_storage) final {
		std::cout << "file to load shapes from:" << filename << std::endl;
    geom_storage.AddShape(std::make_shared<BasicShape>(
      42, Box3D({{0.0f, 10.0f}, {0.0f, 10.0f}, {0.0f, 0.2f}})
				));
		geom_storage.AddShape(std::make_shared<BasicShape>(
				43, Box3D({{-15.0f, -8.0f}, {-9.0f, 1.0f}, {0.0f, 0.2f}})
		));
		geom_storage.AddShape(std::make_shared<BasicShape>(
				44, Box3D({{-3.0f, 8.0f}, {-15.0f, -7.0f}, {0.0f, 0.2f}})
		));
		return true;
  }
  // Сan we use std::ostream instead?
  bool LoadHeatmap(const std::string& filename, HeatmapStorage& heatmap_storage) final {
    std::cout << "heatmap loading..." << std::endl;
    return true;
  }
  bool LoadMetadata(const std::string& filename, MetadataStorage& metadata_storage) final {
    std::cout << "metadata loading..." << std::endl;
    return true;
  }
};
