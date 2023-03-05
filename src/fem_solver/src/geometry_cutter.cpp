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

FsDatapack GeometryCutter::PrepareTestGeometry() {
	std::cout << "triangulating geometry..." << std::endl;
	std::cout << "converting geometry to db..." << std::endl;
	FsDatapack result;
	const int num_of_shapes = 4; // from 1 to 4
	std::vector<VerticeIndexes::VerticeIndex> indexes_global;
	if (num_of_shapes >= 1) {
		Point3D point_0; point_0.coords[X] = 0; point_0.coords[Y] = 0; point_0.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_0));
		Point3D point_1; point_1.coords[X] = 1; point_1.coords[Y] = 0; point_1.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_1));
		Point3D point_2; point_2.coords[X] = 0; point_2.coords[Y] = 1; point_2.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_2));
		Point3D point_3; point_3.coords[X] = 1; point_3.coords[Y] = 0; point_3.coords[Z] = 1;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_3));
	}
	if (num_of_shapes >= 2) {
		Point3D point_4; point_4.coords[X] = 1; point_4.coords[Y] = 1; point_4.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_4));
	}
	if (num_of_shapes >= 3) {
		Point3D point_5; point_5.coords[X] = 1; point_5.coords[Y] = 1; point_5.coords[Z] = 1;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_5));
	}
	if (num_of_shapes >= 4) {
		Point3D point_6; point_6.coords[X] = 0; point_6.coords[Y] = 2; point_6.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_6));
	}
	const double ambient_temperature = 20;
	const double thermal_conductivity = 149; // for silicium 149
	const double heat_flow = 0.1;
	const double convective_heat = 0.1; // FIXME crash if 0
	const double intensity_of_heat_source = 0;
	if (num_of_shapes >= 1) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = { 0,1,2,3 };
		// inner sides indexes: 3
		std::array<bool, 4> convective_presense_per_side = { true, true, true, false };
		std::array<bool, 4> heat_flow_presense_per_side = { false, false, false, false };
		result.AddElement(new VarianceTetraeder(
			thermal_conductivity, // thermal_conductivity
			ambient_temperature,  // ambient_temperature
			heat_flow, // heat_flow
			5, // intensity_of_heat_source, set 20 to middle element
			convective_heat, // convective_heat
			indexes,
			convective_presense_per_side,
			heat_flow_presense_per_side,
			GetVerticeIndexes()));
	}
	if (num_of_shapes >= 2) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = { 1,2,3,4 };
		// inner sides indexes: 0 3 
		// heat flow on index: 2
		std::array<bool, 4> convective_presense_per_side = { false, true, false, false };
		std::array<bool, 4> heat_flow_presense_per_side = { false, false, true, false };
		result.AddElement(new VarianceTetraeder(
			thermal_conductivity, // thermal_conductivity
			ambient_temperature,  // ambient_temperature
			heat_flow, // heat_flow
			intensity_of_heat_source, // intensity_of_heat_source, set 20 to middle element
			convective_heat, // convective_heat
			indexes,
			convective_presense_per_side,
			heat_flow_presense_per_side,
			GetVerticeIndexes()));
	}
	if (num_of_shapes >= 3) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = { 2,3,4,5 };
		// inner sides indexes: 0 2
		// heat flow on index: 3 
		std::array<bool, 4> convective_presense_per_side = { false, true, false, false };
		std::array<bool, 4> heat_flow_presense_per_side = { false, false, false, true };
		result.AddElement(new VarianceTetraeder(
			thermal_conductivity, // thermal_conductivity
			ambient_temperature,  // ambient_temperature
			heat_flow, // heat_flow
			intensity_of_heat_source, // intensity_of_heat_source, set 20 to middle element
			convective_heat, // convective_heat
			indexes,
			convective_presense_per_side,
			heat_flow_presense_per_side,
			GetVerticeIndexes()));
	}
	if (num_of_shapes >= 4) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = { 2,4,5,6 };
		// inner sides indexes: 0
		std::array<bool, 4> convective_presense_per_side = { false, true, true, true };
		std::array<bool, 4> heat_flow_presense_per_side = { false, false, false, false };
		result.AddElement(new VarianceTetraeder(
			thermal_conductivity, // thermal_conductivity
			ambient_temperature,  // ambient_temperature
			heat_flow, // heat_flow
			intensity_of_heat_source, // intensity_of_heat_source, set 20 to middle element
			convective_heat, // convective_heat
			indexes,
			convective_presense_per_side,
			heat_flow_presense_per_side,
			GetVerticeIndexes()));
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
