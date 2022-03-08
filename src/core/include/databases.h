#ifndef DATABASES_H
#define DATABASES_H

#include <memory>
#include <vector>

#include "heatmap.h"
#include "shapes.h"

template <class Shape>
class GeomStorage {
 public:
	using ShapePtr = std::shared_ptr<Shape>;
	using Shapes   = std::vector<ShapePtr>;
	GeomStorage()  = default;
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

// class with net and temperatures
// TODO: Fix it in future to make handy
class HeatmapStorage {
 public:
	using Heatmaps   = std::vector<Heatmap>;
	HeatmapStorage() = default;
	HeatmapStorage(std::vector<float> x_steps, std::vector<float> y_steps,
	               const std::vector<float>& temperature);

	// NH MH - size of layer
	size_t layers_count_ = 0;  // IST in T2D file
	std::vector<float> x_steps_;
	std::vector<float> y_steps_;
	Heatmaps heatmaps_;
};

class MetadataStorage {
 public:
	MetadataStorage() = default;
};

class FileRepresentation {
 public:
	using Layers = std::vector<Layer>;
	using Shapes = GeomStorage<BasicShape>::Shapes;
	FileRepresentation(RepresentationId id) : id_(id) {}
	// FIXME implement geom search, now we return all shapes
	const Shapes& GetShapes(const Box3D& area = Box3D()) const;
	MetadataPack GetShapeMetadata(Id id) const;
	const Box3D design_borders() const { return design_borders_; }
	GeomStorage<BasicShape>& geom_storage() { return geom_storage_; }
	HeatmapStorage& heatmaps() { return heatmaps_; }
	RepresentationId id() const { return id_; }

 private:
	const RepresentationId id_;
	GeomStorage<BasicShape> geom_storage_;
	Layers layers_;
	HeatmapStorage heatmaps_;
	MetadataStorage metadata_storage_;
	Box3D design_borders_;
};

#endif  // DATABASES_H
