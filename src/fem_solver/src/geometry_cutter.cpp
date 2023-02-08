#include "geometry_cutter.hpp"

#include <cinolib/octree.h>

#include "../../mesh_processor/include/generate_mesh.h"
#include "variance_solver.hpp"

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep) {
	auto heat_data  = file_rep.shapes_metadata();
	auto tet_meshes = MeshProcessor::generate(file_rep.layers());
	assert(heat_data.size() == tet_meshes.size() &&
	       "Heat data and shapes mismatch");

	FsDatapack result;

	MeshProcessor::CustomTetmesh total_tetmesh;
	for (auto i = 0; i < tet_meshes.size(); ++i) {
		const auto shape_heat_data = heat_data[i];
		auto& tet_mesh             = tet_meshes[i];
		auto mesh_volume           = tet_mesh.mesh_volume();
		auto& polys                = tet_mesh.vector_polys();
		for (auto pid = 0; pid < polys.size(); ++pid) {
			auto& p_data                = tet_mesh.poly_data(pid);
			p_data.thermal_conductivity = shape_heat_data.thermal_conductivity;
			p_data.ambient_temperature  = shape_heat_data.ambient_temperature;
			p_data.heat_flow            = shape_heat_data.heat_flow;
			p_data.intensity_of_heat_source =
					shape_heat_data.power * tet_mesh.poly_volume(pid) / mesh_volume;
			p_data.convective_heat = shape_heat_data.convective_heat;
		}
		total_tetmesh += tet_mesh;
	}
	/*std::cout << "triangulating geometry..." << std::endl;
	std::cout << "converting geometry to db..." << std::endl;
*/
	const auto& polys = total_tetmesh.vector_polys();
	for (auto pid = 0; pid < polys.size(); ++pid) {
		std::array<VerticeIndexes::VerticeIndex, 4> indexes;
		std::copy(polys[pid].begin(), polys[pid].end(), indexes.begin());
		const auto& p_data = total_tetmesh.poly_data(pid);
		
		result.AddElement(new VarianceTetraeder(
				p_data.thermal_conductivity, p_data.ambient_temperature,
				p_data.heat_flow, p_data.intensity_of_heat_source,
				p_data.convective_heat, indexes, GetVerticeIndexes()));
	}

	return result;
}

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}
