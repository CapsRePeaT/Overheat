#define pragma once

#include <cinolib/color.h>
#include <cinolib/meshes/drawable_tetmesh.h>
#include <cinolib/meshes/mesh_attributes.h>

#include "../../core/include/databases.h"

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

TetmeshVec generate(const LayersShapes& layers);
}  // namespace MeshProcessor
