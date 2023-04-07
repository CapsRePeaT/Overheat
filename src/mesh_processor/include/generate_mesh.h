#define pragma once

#include <cinolib/color.h>
#include <cinolib/meshes/drawable_tetmesh.h>
#include <cinolib/meshes/mesh_attributes.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/volume_mesh_controls.h>

#include "../../core/include/databases.h"
#include "../../fem_solver/src/geometry_cutter.hpp"
#include "box_mesh.h"

enum ConstraintSide { xy, xy_z, xz, xz_y, yz, yz_x };
using LayerMehses  = std::vector<MeshProcessor::BoxMesh>;
using LayersMehses = std::vector<LayerMehses>;
using TrimeshVec   = std::vector<cinolib::DrawableTrimesh<>>;
using SideBoundaryConditionsMap =
		std::unordered_map<ConstraintSide, std::set<cinolib::vec3d>>;

namespace MeshProcessor {

struct Polyhedron_attributes {
	cinolib::Color color = cinolib::Color::WHITE();
	int label            = -1;
	float quality        = 0.0;
	std::bitset<8> flags = 0x00;

	double thermal_conductivity = 0;
	double intensity_of_heat_source = 0;  // мощность пропорциональна обьему
	                                      // power* total_colume/n_tethra
	GlobalId shape_id;
	CornerConditions corner_conditions;
};

using CustomTetmesh = cinolib::DrawableTetmesh<
		cinolib::Mesh_std_attributes, cinolib::Vert_std_attributes,
		cinolib::Edge_std_attributes, cinolib::Polygon_std_attributes,
		Polyhedron_attributes>;

using TetmeshVec = std::vector<CustomTetmesh>;

inline bool operator>(const cinolib::vec3d& lhs, const cinolib::vec3d& rhs) {
	return std::tie(lhs.x(), lhs.y(), lhs.z()) >
	       std::tie(rhs.x(), rhs.y(), rhs.z());
}

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
	SideBoundaryConditionsMap get_boundary_verts() { return boundary_verts_; };
	template <class DrawableMesh>
	static void show_debug_mesh(DrawableMesh& mesh);
 private:
	TetmeshVec generate_layers_meshes(const LayersShapes& layers, bool show_mesh);
	TetmeshVec generate_tetmesh_from_trimeshes(TrimeshVec& meshes,
	                                           bool show_mesh);
	CustomTetmesh generate_tetmesh(const cinolib::DrawableTrimesh<>& mesh);
	TrimeshVec generate_trimesh_from_layers(LayersMehses& layers);
	void calculate_mesh_and_translate_to_origin_pos(LayersMehses& layers);
	void calculate_holes_for_boxes(LayersMehses& layers);

	FileRepresentation& representation_;
	MeshConstraintFunction area_constraint_;
	MeshConstraintFunction volume_constraint_;
	std::optional<double> corner_points_step_;
	SideBoundaryConditionsMap boundary_verts_;
};

template <class DrawableMesh>
void MeshGenerator::show_debug_mesh(DrawableMesh& mesh) {
	using namespace cinolib;
	GLcanvas gui(1920, 980);
	int scale_factor = 1000;
	mesh.updateGL();
	DrawableArrow x(vec3d(-10 * scale_factor, 0, 0), vec3d(10 * scale_factor, 0, 0));
	x.color = Color::GREEN();
	x.size = 0.1 * scale_factor;
	DrawableArrow y(vec3d(0, -10 * scale_factor, 0), vec3d(0, 10 * scale_factor, 0));
	y.color = Color::BLUE();
	y.size = 0.1 * scale_factor;
	DrawableArrow z(vec3d(0, 0, -10 * scale_factor), vec3d(0, 0, 10 * scale_factor));
	z.size = 0.1 * scale_factor;

	// for cutting geometry and look inside
	// MeshSlicer slicer;
	// slicer.X_thresh = 0.6f; // in percents
	// slicer.slice(mesh);
	// mesh.updateGL();
	gui.push(&mesh);
	gui.push(&x);
	gui.push(&y);
	gui.push(&z);
	gui.launch();
}
}  // namespace MeshProcessor
