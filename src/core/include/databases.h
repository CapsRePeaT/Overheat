#ifndef DATABASES_H
#define DATABASES_H

#include "heatmap.h"
#include "shapes.h"

#include <memory>
#include <vector>

template <class Shape>
class GeomStorage {
	
 public:
  using ShapePtr = std::shared_ptr<Shape>;
	using Shapes = std::vector<ShapePtr>;
	GeomStorage() = default;
	void AddShape(ShapePtr shape) { shapes_.emplace_back(std::move(shape)); }
	const Shapes& get_all_shapes() const { return shapes_; }
	void Clear() { shapes_.clear(); }

 private:
	Shapes shapes_;
};

// not sure that we need it now as we have only one heatmap
class Layer {
 public:
	Layer() = default;

 private:
	size_t id_;
	size_t top_heatmap_id_;
	size_t bottom_heatmap_id_;
};

class MetadataStorage {
 public:
	MetadataStorage() = default;
};

class FileRepresentation {
 public:
	using Layers = std::vector<Layer>;
	using Heatmaps = std::vector<Heatmap>;
  using Shapes = GeomStorage<BasicShape>::Shapes;
	// FIXME implement geom search, now we return all shapes
	const Shapes& GetShapes(const Box3D& area = Box3D()) const;
	const Box3D design_borders() const { return design_borders_; }
	GeomStorage<BasicShape>& geom_storage() { return geom_storage_; }

 private:
	GeomStorage<BasicShape> geom_storage_;
	Layers layers_;
	Heatmaps heatmaps_;
	MetadataStorage metadata_storage_;
	Box3D design_borders_;
};

#endif  // DATABASES_H
