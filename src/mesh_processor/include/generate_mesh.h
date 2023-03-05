#define pragma once

#include <cinolib/color.h>
#include <cinolib/meshes/drawable_tetmesh.h>
#include <cinolib/meshes/mesh_attributes.h>

#include "../../core/include/databases.h"
#include "../../fem_solver/src/geometry_cutter.hpp"
#include "box_mesh.h"

using LayerMehses  = std::vector<MeshProcessor::BoxMesh>;
using LayersMehses = std::vector<LayerMehses>;
using TrimeshVec   = std::vector<cinolib::DrawableTrimesh<>>;

namespace MeshProcessor {

struct Polyhedron_attributes {
	cinolib::Color color = cinolib::Color::WHITE();
	int label            = -1;
	float quality        = 0.0;
	std::bitset<8> flags = 0x00;

	double thermal_conductivity = 0;
	double ambient_temperature  = 0;  // env_thermal_conductivity_
	double heat_flow            = 0;  // уточнить
	double intensity_of_heat_source = 0;  // мощность пропорциональна обьему
	                                      // power* total_colume/n_tethra
	double convective_heat = 0;
};

using CustomTetmesh = cinolib::DrawableTetmesh<
		cinolib::Mesh_std_attributes, cinolib::Vert_std_attributes,
		cinolib::Edge_std_attributes, cinolib::Polygon_std_attributes,
		Polyhedron_attributes>;

using TetmeshVec = std::vector<CustomTetmesh>;

class MeshGenerator {
 public:
	MeshGenerator(FileRepresentation& file_rep,
	              MeshConstraintFunction area_constraint,
	              MeshConstraintFunction volume_constraint,
	              const std::optional<double> corner_points_step)
			: representation_(file_rep),
				area_constraint_(area_constraint),
				volume_constraint_(volume_constraint),
				corner_points_step_(corner_points_step) {}

	CustomTetmesh get_tetmesh(bool show_mesh = true);

 private:
	TetmeshVec generate_layers_meshes(const LayersShapes& layers, bool show_mesh);
	TetmeshVec generate_tetmesh_from_trimeshes(TrimeshVec& meshes, bool show_mesh);
	CustomTetmesh generate_tetmesh(const cinolib::DrawableTrimesh<>& mesh);
	TrimeshVec generate_trimesh_from_layers(LayersMehses& layers);
	void calculate_mesh_and_translate_to_origin_pos(LayersMehses& layers);
	void calculate_holes_for_boxes(LayersMehses& layers);

	FileRepresentation& representation_;
	MeshConstraintFunction area_constraint_;
	MeshConstraintFunction volume_constraint_;
	std::optional<double> corner_points_step_;
};
}  // namespace MeshProcessor
