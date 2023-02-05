#include "geometry_cutter.hpp"
#include "variance_solver.hpp"

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep) {
	std::cout << "triangulating geometry..." << std::endl;
	std::cout << "converting geometry to db..." << std::endl;
	FsDatapack result;
	const int num_of_shapes = 1;
	for (int i = 0; i < num_of_shapes; ++i) {
		std::array<Point3D, 4> points;
		std::array<VerticeIndexes::VerticeIndex, 4> indexes;
		std::array<bool, 4> convective_presense_per_side;
		std::array<bool, 4> heat_flow_presense_per_side;
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 3; ++k)
				points[j].coords[k] = i * 12 + j * 3 + k;
			// to avoid 0 determinants
			points[0].coords[0] = 5; 
			points[3].coords[2] = 10;
			indexes[j] = GetVerticeIndexes().AddVertice(points[j]);
			convective_presense_per_side[j] = true;
			heat_flow_presense_per_side[j] = true;
		}
		result.AddElement(new VarianceTetraeder(
			149, // thermal_conductivity
			20,  // ambient_temperature
			0.2, // heat_flow
			30, // intensity_of_heat_source
			0.7, // convective_heat
			indexes, 
			convective_presense_per_side,
			heat_flow_presense_per_side,
			GetVerticeIndexes()));
	}
	return result;
}

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() const {
	return index_to_coord_map_;
}

VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}
