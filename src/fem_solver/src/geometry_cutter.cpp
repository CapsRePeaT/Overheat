#include "geometry_cutter.hpp"

#include <cinolib/octree.h>

#include <iostream>

#include "../../mesh_processor/include/generate_mesh.h"
#include "cinolib/drawable_arrow.h"
#include "cinolib/gl/glcanvas.h"
#include "variance_solver.hpp"

namespace {
void show_debug_mesh(MeshProcessor::CustomTetmesh& mesh) {
	using namespace cinolib;
	GLcanvas gui(1920, 980);
	mesh.updateGL();
	DrawableArrow x(vec3d(-35000, 0, 0), vec3d(35000, 0, 0));
	x.color = Color::GREEN();
	x.size  = 10;
	DrawableArrow y(vec3d(0, -35000, 0), vec3d(0, 35000, 0));
	y.color = Color::BLUE();
	y.size  = 10;
	DrawableArrow z(vec3d(0, 0, -35000), vec3d(0, 0, 35000));
	z.size = 10;

	// for cutting geometry and look inside
	//MeshSlicer slicer;
	//slicer.X_thresh = 0.6f; // in percents
	//slicer.slice(mesh);
	//mesh.updateGL();


	gui.push(&mesh);
	gui.push(&x);
	gui.push(&y);
	gui.push(&z);
	gui.launch();
}
}

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep,
                                           bool show_mesh) {
	auto generator = MeshProcessor::MeshGenerator(
			file_rep, area_constraint_, volume_constraint_, corner_points_step_);
	auto total_tetmesh = generator.get_tetmesh();
	if (show_mesh)
		show_debug_mesh(total_tetmesh);

	const auto upper_point_z = total_tetmesh.bbox().delta_z();

	const auto& polys = total_tetmesh.vector_polys();
	const auto& verts = total_tetmesh.vector_verts();

	FsDatapack result;
	std::array<std::array<int, 3>, 4> faces_indexes = {
			std::array<int, 3>{0, 1, 2}, std::array<int, 3>{0, 1, 3},
			std::array<int, 3>{0, 2, 3}, std::array<int, 3>{1, 2, 3}};

	for (auto pid = 0; pid < polys.size(); ++pid) {
		const auto& p_data = total_tetmesh.poly_data(pid);

		std::array<VerticeIndexes::VerticeIndex, 4> indexes;
		std::array<cinolib::vec3d, 4> coord;
		auto poly_verts = total_tetmesh.poly_verts(pid);
		for (auto i = 0; i < polys[pid].size(); ++i) {
			auto v        = poly_verts[i];
			auto glob_ind = GetVerticeIndexes().AddVertice({{v.x(), v.y(), v.z()}});
			indexes[i]    = glob_ind;
			coord[i]      = v;
		}

		std::array<bool, 4> convective_presense_per_side;
		for (auto face_ind = 0; face_ind < faces_indexes.size(); ++face_ind) {
			auto faces_verts_indexes = faces_indexes[face_ind];
			auto is_upper_face       = true;
			for (auto vert_ind : faces_verts_indexes) {
				auto vert     = coord[vert_ind];
				is_upper_face = is_upper_face && vert.z() == upper_point_z;
			}
			convective_presense_per_side[face_ind] = is_upper_face;
		}

		std::array<bool, 4> heat_flow_presense_per_side = {0, 0, 0, 0};  // 7493
		result.AddElement(new VarianceTetraeder(
				p_data.thermal_conductivity, p_data.ambient_temperature,
				p_data.heat_flow, p_data.intensity_of_heat_source,
				p_data.convective_heat, indexes, convective_presense_per_side,
				heat_flow_presense_per_side, GetVerticeIndexes()));
	}

	return result;
}

VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() const {
	return index_to_coord_map_;
}
GeometryCutter::GeometryCutter(double corner_points_step,
                               MeshConstraintFunction area_constraint,
                               MeshConstraintFunction volume_constraint)
		: corner_points_step_(corner_points_step),
			area_constraint_(area_constraint),
			volume_constraint_(volume_constraint) {}

GeometryCutter::GeometryCutter(MeshConstraintFunction area_constraint,
                               MeshConstraintFunction volume_constraint)
		: area_constraint_(area_constraint),
			volume_constraint_(volume_constraint) {}
