#pragma once

#include <string>
#include <vector>

#include "../readers/IReader.h"
#include "common.h"

class Core {
 public:
  static Core& instance() { static Core instance; return instance; }
  void LoadHeatmap(const std::string& file_name) {}
  void LoadGeometry(const std::string& file_name) {}
  std::vector<BasicShape*> LoadShapes(const BBox3D& area = BBox3D());
  const BBox3D design_borders() { return design_borders_; }
  // get metadata by id func

 private:
  Core() = default;
  GeomStorage<BasicShape*> geom_storage_;
  HeatmapStorage heatmap_storage_;
  MetadataStorage metadata_storage_;
  BBox3D design_borders_;
};
