#include "geometry_cutter.hpp"

#include <cinolib/octree.h>

#include <algorithm>
#include <cassert>
#include <iostream>

#include "../../mesh_processor/include/generate_mesh.h"
#include "cinolib/drawable_arrow.h"
#include "cinolib/gl/glcanvas.h"
#include "variance_solver.hpp"

namespace {
bool is_boundary_face(std::set<cinolib::vec3d>& boundary,
                      std::array<cinolib::vec3d, 3>& face_coords) {
	return std::all_of(
			face_coords.begin(), face_coords.end(),
			[&boundary](const auto vert) { return boundary.contains(vert); });
}

std::optional<CornerCondition> get_boundary_conds(
		MeshProcessor::Polyhedron_attributes& p_data,
		SideBoundaryConditionsMap& boundary,
		std::array<cinolib::vec3d, 3>& face_coords,
		std::array<cinolib::vec3d, 4>& poly_coords) {
	double min_x = std::min_element(poly_coords.begin(), poly_coords.end(),
	                                [](auto f, auto s) { return f.x() < s.x(); })
	                   ->x();
	double max_x = std::max_element(poly_coords.begin(), poly_coords.end(),
	                                [](auto f, auto s) { return f.x() < s.x(); })
	                   ->x();

	double min_y = std::min_element(poly_coords.begin(), poly_coords.end(),
	                                [](auto f, auto s) { return f.y() < s.y(); })
	                   ->y();
	double max_y = std::max_element(poly_coords.begin(), poly_coords.end(),
	                                [](auto f, auto s) { return f.y() < s.y(); })
	                   ->y();

	double min_z = std::min_element(poly_coords.begin(), poly_coords.end(),
	                                [](auto f, auto s) { return f.z() < s.z(); })
	                   ->z();
	double max_z = std::max_element(poly_coords.begin(), poly_coords.end(),
	                                [](auto f, auto s) { return f.z() < s.z(); })
	                   ->z();

	for (auto& b : boundary)
		if (is_boundary_face(b.second, face_coords)) {
			switch (b.first) {
				case ConstraintSide::xy:
					if (face_coords[0].z() == face_coords[1].z() &&
					    face_coords[1].z() == face_coords[2].z() &&
					    face_coords[0].z() == min_z)
						return p_data.corner_conditions.xy;
					break;
				case ConstraintSide::xy_z:
					if (face_coords[0].z() == face_coords[1].z() &&
					    face_coords[1].z() == face_coords[2].z() &&
					    face_coords[0].z() == max_z)
						return p_data.corner_conditions.xy_z;
					break;
				case ConstraintSide::xz:
					if (face_coords[0].y() == face_coords[1].y() &&
					    face_coords[1].y() == face_coords[2].y() &&
					    face_coords[0].y() == min_y)
						return p_data.corner_conditions.xz;
					break;
				case ConstraintSide::xz_y:
					if (face_coords[0].y() == face_coords[1].y() &&
					    face_coords[1].y() == face_coords[2].y() &&
					    face_coords[0].y() == max_y)
						return p_data.corner_conditions.xz_y;
					break;
				case ConstraintSide::yz:
					if (face_coords[0].x() == face_coords[1].x() &&
					    face_coords[1].x() == face_coords[2].x() &&
					    face_coords[0].x() == min_x)
						return p_data.corner_conditions.yz;
					break;
				case ConstraintSide::yz_x:
					if (face_coords[0].x() == face_coords[1].x() &&
					    face_coords[1].x() == face_coords[2].x() &&
					    face_coords[0].x() == max_x)
						return p_data.corner_conditions.yz_x;
					break;
				default:
					throw std::runtime_error("Unexpected constraint");
			}
		}
	return std::nullopt;
};

}  // namespace

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep,
                                           bool show_mesh) {
	using namespace MeshProcessor;
	auto timer_start = std::chrono::high_resolution_clock::now();
	auto generator = MeshGenerator(file_rep, area_constraint_, volume_constraint_,
	                               corner_points_step_);
	auto total_tetmesh            = generator.get_tetmesh(show_mesh);
	auto boundary                 = generator.get_boundary_verts();
	auto timer_get_tetmesh_finish = std::chrono::high_resolution_clock::now();
	std::cout << "Tet gen fineshed, it took "
						<< std::chrono::duration_cast<std::chrono::seconds>(
									 timer_get_tetmesh_finish - timer_start)
						<< " seconds." << std::endl;

	auto ambient_temperature = file_rep.ambient_temperature();

	const auto upper_point_z = total_tetmesh.bbox().delta_z();

	const auto& polys = total_tetmesh.vector_polys();
	const auto& verts = total_tetmesh.vector_verts();

	FsDatapack result;
	std::array<std::array<int, 3>, 4> faces_indexes = {
			std::array<int, 3>{0, 1, 2}, std::array<int, 3>{0, 1, 3},
			std::array<int, 3>{0, 2, 3}, std::array<int, 3>{1, 2, 3}};

	for (auto pid = 0; pid < polys.size(); ++pid) {
		auto& p_data = total_tetmesh.poly_data(pid);

		std::array<VerticeIndexes::VerticeIndex, 4> indexes;
		std::array<cinolib::vec3d, 4> poly_coords;

		auto poly_verts = total_tetmesh.poly_verts(pid);
		assert(polys[pid].size() == 4);
		for (auto i = 0; i < polys[pid].size(); ++i) {
			auto v        = poly_verts[i];
			auto glob_ind = GetVerticeIndexes().AddVertice(
					{v.x() / 1000, v.y() / 1000, v.z() / 1000});  //  scale verts
			indexes[i]     = glob_ind;
			poly_coords[i] = v;
		}

		std::array<double, 4> convective_presense_per_side = {0, 0, 0, 0};
		std::array<double, 4> heat_flow_presense_per_side  = {0, 0, 0, 0};
		for (auto face_ind = 0; face_ind < faces_indexes.size(); ++face_ind) {
			auto faces_verts_indexes                  = faces_indexes[face_ind];
			std::array<cinolib::vec3d, 3> face_coords = {
					poly_coords[faces_verts_indexes[0]],
					poly_coords[faces_verts_indexes[1]],
					poly_coords[faces_verts_indexes[2]]};

			auto boundary_conds =
					get_boundary_conds(p_data, boundary, face_coords, poly_coords);
			if (boundary_conds.has_value()) {
				convective_presense_per_side[face_ind] =
						boundary_conds->convective_heat;
				heat_flow_presense_per_side[face_ind] = boundary_conds->heat_flow;
				if (boundary_conds->temperature > 0)
					for (auto coord : face_coords) {
						points_temps_.insert({boundary_conds->temperature,
						                      Point3D{coord.x() / 1000, coord.y() / 1000,
						                              coord.z() / 1000}});
					}
			}
		}
		result.AddElement(new VarianceTetraeder(
				p_data.shape_id,
				p_data.thermal_conductivity, ambient_temperature,
				p_data.intensity_of_heat_source, indexes, convective_presense_per_side,
				heat_flow_presense_per_side, GetVerticeIndexes()));
	}
	auto element_contribution_finish = std::chrono::high_resolution_clock::now();
	std::cout << "Element contribution counting fineshed, it took "
						<< std::chrono::duration_cast<std::chrono::seconds>(
									 element_contribution_finish - timer_get_tetmesh_finish)
						<< " seconds." << std::endl;

	return result;
}

FsDatapack GeometryCutter::PrepareTestGeometry() {
	std::cout << "triangulating geometry..." << std::endl;
	std::cout << "converting geometry to db..." << std::endl;
	FsDatapack result;
	const int num_of_shapes = 4;  // from 1 to 4
	std::vector<VerticeIndexes::VerticeIndex> indexes_global;
	if (num_of_shapes >= 1) {
		Point3D point_0;
		point_0.coords[X] = 0;
		point_0.coords[Y] = 0;
		point_0.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_0));
		Point3D point_1;
		point_1.coords[X] = 1;
		point_1.coords[Y] = 0;
		point_1.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_1));
		Point3D point_2;
		point_2.coords[X] = 0;
		point_2.coords[Y] = 1;
		point_2.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_2));
		Point3D point_3;
		point_3.coords[X] = 1;
		point_3.coords[Y] = 0;
		point_3.coords[Z] = 1;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_3));
	}
	if (num_of_shapes >= 2) {
		Point3D point_4;
		point_4.coords[X] = 1;
		point_4.coords[Y] = 1;
		point_4.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_4));
	}
	if (num_of_shapes >= 3) {
		Point3D point_5;
		point_5.coords[X] = 1;
		point_5.coords[Y] = 1;
		point_5.coords[Z] = 1;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_5));
	}
	if (num_of_shapes >= 4) {
		Point3D point_6;
		point_6.coords[X] = 0;
		point_6.coords[Y] = 2;
		point_6.coords[Z] = 0;
		indexes_global.push_back(GetVerticeIndexes().AddVertice(point_6));
	}
	const double ambient_temperature      = 20;
	const double thermal_conductivity     = 149;  // for silicium 149
	const double heat_flow                = 0.1;
	const double convective_heat          = 0.1;  // FIXME crash if 0
	const double intensity_of_heat_source = 0;
	GlobalId shape_id;
	if (num_of_shapes >= 1) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = {0, 1, 2, 3};
		// inner sides indexes: 3
		std::array<double, 4> convective_presense_per_side = {
				convective_heat, convective_heat, convective_heat, 0};
		std::array<double, 4> heat_flow_presense_per_side = {0, 0, 0, 0};
		result.AddElement(new VarianceTetraeder(
				shape_id,
				thermal_conductivity,  // thermal_conductivity
				ambient_temperature,   // ambient_temperature
				5,  // intensity_of_heat_source, set 20 to middle element
				indexes, convective_presense_per_side, heat_flow_presense_per_side,
				GetVerticeIndexes()));
	}
	if (num_of_shapes >= 2) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = {1, 2, 3, 4};
		// inner sides indexes: 0 3
		// heat flow on index: 2
		std::array<double, 4> convective_presense_per_side = {0, convective_heat, 0,
		                                                      0};
		std::array<double, 4> heat_flow_presense_per_side  = {0, 0, heat_flow, 0};
		result.AddElement(new VarianceTetraeder(
				shape_id,
				thermal_conductivity,      // thermal_conductivity
				ambient_temperature,       // ambient_temperature
				intensity_of_heat_source,  // intensity_of_heat_source, set 20 to middle
				indexes, convective_presense_per_side, heat_flow_presense_per_side,
				GetVerticeIndexes()));
	}
	if (num_of_shapes >= 3) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = {2, 3, 4, 5};
		// inner sides indexes: 0 2
		// heat flow on index: 3
		std::array<double, 4> convective_presense_per_side = {0, convective_heat, 0,
		                                                      0};
		std::array<double, 4> heat_flow_presense_per_side  = {0, 0, 0, heat_flow};
		result.AddElement(new VarianceTetraeder(
				shape_id,
				thermal_conductivity,      // thermal_conductivity
				ambient_temperature,       // ambient_temperature
				intensity_of_heat_source,  // intensity_of_heat_source, set 20 to middle
				indexes, convective_presense_per_side, heat_flow_presense_per_side,
				GetVerticeIndexes()));
	}
	if (num_of_shapes >= 4) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = {2, 4, 5, 6};
		// inner sides indexes: 0
		std::array<double, 4> convective_presense_per_side = {
				0, convective_heat, convective_heat, convective_heat};
		std::array<double, 4> heat_flow_presense_per_side = {0, 0, 0, 0};
		result.AddElement(new VarianceTetraeder(
				shape_id,
				thermal_conductivity,      // thermal_conductivity
				ambient_temperature,       // ambient_temperature
				intensity_of_heat_source,  // intensity_of_heat_source, set 20 to middle
				indexes, convective_presense_per_side, heat_flow_presense_per_side,
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
