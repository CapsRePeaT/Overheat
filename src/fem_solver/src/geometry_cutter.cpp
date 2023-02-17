#include "geometry_cutter.hpp"

#include <cinolib/octree.h>

#include "../../mesh_processor/include/generate_mesh.h"
#include "variance_solver.hpp"
#include <iostream>

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep) {
	auto generator     = MeshProcessor::MeshGenerator(file_rep, 500000, 500000);
	auto total_tetmesh = generator.get_tetmesh();

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
			auto v = poly_verts[i];
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

		std::array<bool, 4> heat_flow_presense_per_side = {0, 0, 0, 0};//7493
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
