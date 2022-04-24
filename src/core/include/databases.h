#pragma once

#include <memory>
#include <vector>

#include "heatmap.h"
#include "shapes.h"
#include "metadata_storage.h"

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
	// we assume that shape id equual to plase of shape in the array.
	Shapes shapes_;
};

class Layer {
 public:
	Layer() = default;

	GlobalId id() const { return id_; } 

 private:
	GlobalId id_;
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
	               const std::vector<float>& temperature, Box3D design_borders);
	[[nodiscard]] Box3D representation_borders() const {
		return representation_borders_;
	}
	[[nodiscard]] float MinStep() const;
	[[nodiscard]] const Heatmaps& heatmaps() const { return heatmaps_; }
	[[nodiscard]] const std::vector<float>& x_steps() const { return x_steps_; }
	[[nodiscard]] const std::vector<float>& y_steps() const { return y_steps_; }
 private:
	size_t layers_count_ = 0;  // IST in T2D file
	std::vector<float> x_steps_;
	std::vector<float> y_steps_;
	Box3D representation_borders_;
	// we duplicate borders here to make manipulation 
	// with heatmap handier, borders needed for proper heatmap interpolation
	Heatmaps heatmaps_;
};

class FileRepresentation {
 public:
	using Layers = std::vector<Layer>;
	using Shapes = GeomStorage<BasicShape>::Shapes;
	FileRepresentation() : id_(InstanceType::Representation, 
		                         0 /*Instanse id*/, 
		                         id_counter++) {}
	FileRepresentation(FileRepresentation&&) = default;
	~FileRepresentation() = default;
	FileRepresentation(const FileRepresentation&) = delete;
	FileRepresentation& operator=(const FileRepresentation&) = delete;
	FileRepresentation& operator=(FileRepresentation&&) = delete; // id_ is const
	// for side widgets
	GobalIds GetAllLayerIds() const;
	GobalIds GetAllShapeIdsOfLayer(GlobalId layer_id) const;

	// FIXME implement geom search, now we return all shapes
	[[nodiscard]] const Shapes& GetShapes(const Box3D& area = Box3D()) const;
	[[nodiscard]] MetadataPack GetShapeMetadata(ShapeId id) const;
	[[nodiscard]] Box3D representation_borders() const {
		return heatmaps_.representation_borders();
	}
	[[nodiscard]] GlobalId id() const { return id_; }
	std::string GetName(GlobalId id) const {
		return metadata_storage_.GetInstanceName(id);
	}
	// needed for geometry loading
	GeomStorage<BasicShape>& geom_storage() { return geom_storage_; }
	HeatmapStorage& heatmaps() { return heatmaps_; }
 private:
	inline static RepresentationId id_counter = 0;

	const GlobalId id_;
	// we assume that shape id equal to place of shape in the array.
	GeomStorage<BasicShape> geom_storage_;
	// we assume that layer id equal to place of shape in the array.
	Layers layers_;
	// we assume that heatmap id equal to place of shape in the array.
	HeatmapStorage heatmaps_;
	DefaultMetadataStorage metadata_storage_;
	Box3D design_borders_;
};
