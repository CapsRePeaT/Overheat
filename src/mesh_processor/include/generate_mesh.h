#define pragma once

#include "box_mesh.h"

#include "../../core/include/databases.h"

#include <cinolib/color.h>
#include <cinolib/meshes/drawable_tetmesh.h>
#include <cinolib/meshes/mesh_attributes.h>



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
	MeshGenerator(FileRepresentation& file_rep, double volume_thresh,
	              double area_thresh)
			: representation_(file_rep),
				volume_thresh_(volume_thresh),
				area_thresh_(area_thresh) {}

	CustomTetmesh get_tetmesh();

 private:
	TetmeshVec generate_layers_meshes(const LayersShapes& layers);
	TetmeshVec generate_tetmesh_from_trimeshes(TrimeshVec& meshes);
	CustomTetmesh generate_tetmesh(const cinolib::DrawableTrimesh<>& mesh);
	TrimeshVec generate_trimesh_from_layers(LayersMehses& layers);
	void calculate_mesh_and_translate_to_origin_pos(LayersMehses& layers);
	void calculate_holes_for_boxes(LayersMehses& layers);

	FileRepresentation& representation_;
	double volume_thresh_;
	double area_thresh_;
};
}  // namespace MeshProce