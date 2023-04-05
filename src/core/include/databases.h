#pragma once

#include <limits>
#include <memory>
#include <vector>

#include "heatmap.h"
#include "metadata_storage.h"
#include "shapes.h"
#include "solver_db.hpp"

using LayersShapes = std::vector<std::vector<BasicShape>>;

struct CornerCondition {
	float heat_flow       = 0;
	float convective_heat = 0;
	float temperature     = 0;
};

struct SolverSetup {
	double corner_points_step = 0.5;
	bool show_triangulation = true;
	bool calculate_test_geometry = false;
};

struct CornerConditions {
	CornerCondition xy;
	CornerCondition xy_z;
	CornerCondition xz;
	CornerCondition xz_y;
	CornerCondition yz;
	CornerCondition yz_x;
};

struct ShapeHeatData {
	double thermal_conductivity        = 0;
	double ambient_temperature         = 0;
	double power                       = 0;
	CornerConditions corner_conditions;
};

using ShapesHeatData = std::vector<ShapeHeatData>;

template <class Shape>
class GeomStorage {
 public:
	using ShapePtr = std::shared_ptr<Shape>;
	using Shapes   = std::vector<ShapePtr>;
	GeomStorage()  = default;
	void AddShape(ShapePtr shape) { shapes_.emplace_back(std::move(shape)); }
	[[nodiscard]] const Shapes& shapes() const { return shapes_; }
	[[nodiscard]] const ShapePtr shape(ShapeId id) const {
		assert(id < shapes_.size());
		return shapes_.at(id);
	}
	const ShapePtr operator[](ShapeId id) const { return shape(id); }
	void Clear() { shapes_.clear(); }

 private:
	// we assume that shape id equal to place of shape in the array.
	Shapes shapes_;
};

class Layer {
 public:
	Layer() = default;
	Layer(const GlobalId id, HeatmapId bottom_heatmap_id,
	      HeatmapId top_heatmap_id)
			: id_(id),
				top_heatmap_id_(top_heatmap_id),
				bottom_heatmap_id_(bottom_heatmap_id) {}

	[[nodiscard]] GlobalId id() const { return id_; }
	HeatmapId top_heatmap_id() const { return top_heatmap_id_; }
	HeatmapId bottom_heatmap_id() const { return bottom_heatmap_id_; }

 private:
	// TODO implement me
	const float bottom_ = std::numeric_limits<float>::max();
	const float width_  = std::numeric_limits<float>::max();
	GlobalId id_;
	HeatmapId top_heatmap_id_;
	HeatmapId bottom_heatmap_id_;
};

// class with net and temperatures
class HeatmapStorage {
 public:
	HeatmapStorage() = default;
	HeatmapStorage(std::vector<float> x_steps, std::vector<float> y_steps,
	               const std::vector<float>& temperature,
	               float environment_temperature, Box3D representation_borders);
	[[nodiscard]] Box3D representation_borders() const {
		return representation_borders_;
	}
	[[nodiscard]] float environment_temperature() const { return env_temp_; }
	const Heatmap& operator[](const size_t id) const {
		assert(id < heatmaps_.size());
		return heatmaps_.at(id);
	}
	[[nodiscard]] float x_size() const {
		return representation_borders_.coordinates()[0].second;
	}
	[[nodiscard]] float y_size() const {
		return representation_borders_.coordinates()[1].second;
	}
	[[nodiscard]] float MinStep() const { return min_step_; }
	[[nodiscard]] float max_temp() const;
	[[nodiscard]] float min_temp() const;
	[[nodiscard]] const Heatmaps& heatmaps() const { return heatmaps_; }
	[[nodiscard]] const Floats& x_coords() const { return x_coords_; }
	[[nodiscard]] const Floats& y_coords() const { return y_coords_; }

 private:
	size_t layers_count_ = 0;  // IST in T2D file
	float env_temp_      = 0.0f;
	float min_step_;
	Floats x_coords_;
	Floats y_coords_;
	Box3D representation_borders_;
	// we duplicate borders here to make manipulation
	// with heatmap handier, borders needed for proper heatmap interpolation
	Heatmaps heatmaps_;

	mutable float min_temp_cache_ = std::numeric_limits<float>::quiet_NaN();
	mutable float max_temp_cache_ = std::numeric_limits<float>::quiet_NaN();

	void SetMinMaxTempCaches() const;
};

class FileRepresentation {
 public:
	using Layers = std::vector<Layer>;
	using Shapes = GeomStorage<BasicShape>::Shapes;
	FileRepresentation(GeomStorage<BasicShape> geom_storage_mv,
	                   HeatmapStorage heatmap_storage_mv,
	                   LayersShapes layers_shapes_mv,
	                   ShapesHeatData shapes_metadata_mv,
	                   float ambient_temperature)
			: id_(InstanceType::Representation, 0 /*Instance id*/, id_counter++),
				geom_storage_(std::move(geom_storage_mv)),
				heatmaps_(std::move(heatmap_storage_mv)),
				layers_shapes_(std::move(layers_shapes_mv)),
				shapes_metadata_(std::move(shapes_metadata_mv)),
				ambient_temperature_(ambient_temperature){}
	FileRepresentation(GeomStorage<BasicShape> geom_storage_mv)
			: id_(InstanceType::Representation, 0 /*Instance id*/, id_counter++),
				geom_storage_(std::move(geom_storage_mv)) {}
	FileRepresentation(FileRepresentation&&)                 = default;
	~FileRepresentation()                                    = default;
	FileRepresentation(const FileRepresentation&)            = default;
	FileRepresentation& operator=(const FileRepresentation&) = delete;
	FileRepresentation& operator=(FileRepresentation&&) = delete;  // id_ is const

	// FIXME implement geom search, now we return all shapes
	[[nodiscard]] const Shapes& GetShapes(const Box3D& area = Box3D()) const;
	[[nodiscard]] MetadataPack GetMetadata(GlobalId id) const;
	[[nodiscard]] Box3D representation_borders() const {
		return heatmaps_.representation_borders();
	}
	[[nodiscard]] GlobalId id() const { return id_; }
	std::string RepresentationName() const { return GetName(id()); }
	// needed for geometry loading
	GeomStorage<BasicShape>& geom_storage() { return geom_storage_; }
	HeatmapStorage& heatmaps() { return heatmaps_; }
	LayersShapes& layers() { return layers_shapes_; }
	ShapesHeatData shapes_metadata() { return shapes_metadata_; }
	float ambient_temperature() {return ambient_temperature_;}
	InstanceList GetInstanceList() const;
	// should be deleted when proper layer reading will be provided
	[[deprecated]] void InitLayers();
	void set_fs_datapack(FsDatapack fs_datapack) { fs_datapack_ = std::move(fs_datapack); }
	const FsDatapack& fs_datapack() const { return fs_datapack_; }
 private:
	// for side widgets
	[[nodiscard]] GlobalIds GetAllLayerIds() const;
	[[nodiscard]] GlobalIds GetAllShapeIdsOfLayer(GlobalId layer_id) const;
	std::string GetName(GlobalId id) const {
		return metadata_storage_.GetInstanceName(id);
	}

	inline static RepresentationId id_counter = 0;
	const GlobalId id_;
	// we assume that shape id equal to place of shape in the array.
	GeomStorage<BasicShape> geom_storage_;
	// we assume that layer id equal to place of shape in the array.
	Layers layers_;
	// we assume that heatmap id equal to place of shape in the array.
	HeatmapStorage heatmaps_;
	// data from solver run
	FsDatapack fs_datapack_;
	DefaultMetadataStorage metadata_storage_;
	LayersShapes layers_shapes_{};
	ShapesHeatData shapes_metadata_{};
	float ambient_temperature_{};
	Box3D design_borders_;
};
