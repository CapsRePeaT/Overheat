#pragma once

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
	[[nodiscard]] const Shapes& shapes() const { return shapes_; }
	void Clear() { shapes_.clear(); }

 private:
	Shapes shapes_;
};

// not sure that we need it now as we have only one heatmap
class Layer {
 public:
	Layer() = default;

 private:
	LayerId id_;
	HeatmapId top_heatmap_id_;
	HeatmapId bottom_heatmap_id_;
};

// class with net and temperatures
// TODO: Fix it in future to make handy
class HeatmapStorage {
 public:
	using Heatmaps   = std::vector<Heatmap>;
	HeatmapStorage() = default;
	HeatmapStorage(std::vector<float> x_steps_mv, std::vector<float> y_steps_mv,
	               const std::vector<float>& temperature);

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
	FileRepresentation() : id_(id_counter++) {}
	FileRepresentation(FileRepresentation&&) = default;
	~FileRepresentation() = default;
	FileRepresentation(const FileRepresentation&) = delete;
	FileRepresentation& operator=(const FileRepresentation&) = delete;
	FileRepresentation& operator=(FileRepresentation&&) = delete; // id_ is const

	// FIXME implement geom search, now we return all shapes
	[[nodiscard]] const Shapes& GetShapes(const Box3D& area = Box3D()) const;
	[[nodiscard]] MetadataPack GetShapeMetadata(ShapeId id) const;
	[[nodiscard]] Box3D design_borders() const { return design_borders_; }
	[[nodiscard]] RepresentationId id() const { return id_; }
	// needed for geometry loading
	GeomStorage<BasicShape>& geom_storage() { return geom_storage_; }
	HeatmapStorage& heatmaps() { return heatmaps_; }
 private:
	inline static RepresentationId id_counter = 0;

	const RepresentationId id_;
	GeomStorage<BasicShape> geom_storage_;
	Layers layers_;
	HeatmapStorage heatmaps_;
	MetadataStorage metadata_storage_;
	Box3D design_borders_;
};
