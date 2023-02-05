#include "geometry_cutter.hpp"
#include "variance_solver.hpp"

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep) {
	std::cout << "triangulating geometry..." << std::endl;
	std::cout << "converting geometry to db..." << std::endl;
	FsDatapack result;
	const int num_of_shapes = 2; //max 3
	std::vector<VerticeIndexes::VerticeIndex> indexes_global;
	Point3D point_0; point_0.coords[X] = 0; point_0.coords[Y] = 0; point_0.coords[Z] = 0;
	indexes_global.push_back(GetVerticeIndexes().AddVertice(point_0));
	Point3D point_1; point_1.coords[X] = 1; point_1.coords[Y] = 0; point_1.coords[Z] = 0;
	indexes_global.push_back(GetVerticeIndexes().AddVertice(point_1));
	Point3D point_2; point_2.coords[X] = 0; point_2.coords[Y] = 1; point_2.coords[Z] = 0;
	indexes_global.push_back(GetVerticeIndexes().AddVertice(point_2));
	Point3D point_3; point_3.coords[X] = 1; point_3.coords[Y] = 0; point_3.coords[Z] = 1;
	indexes_global.push_back(GetVerticeIndexes().AddVertice(point_3));
	Point3D point_4; point_4.coords[X] = 1; point_4.coords[Y] = 1; point_4.coords[Z] = 0;
	indexes_global.push_back(GetVerticeIndexes().AddVertice(point_4));
	//Point3D point_5; point_5.coords[X] = 1; point_5.coords[Y] = 1; point_5.coords[Z] = 1;
	//indexes_global.push_back(GetVerticeIndexes().AddVertice(point_5));

	for (int i = 0; i < num_of_shapes; ++i) {
		std::array<Point3D, 4> points;
		std::array<VerticeIndexes::VerticeIndex, 4> indexes;
		std::array<bool, 4> convective_presense_per_side;
		std::array<bool, 4> heat_flow_presense_per_side;
		for (int j = 0; j < 4; ++j) {
			// shift by 1 index per shape
			indexes[j] = indexes_global[i + j];
			convective_presense_per_side[j] = true;
			heat_flow_presense_per_side[j] = true;
		}
		const double ambient_temperature = 20;
		const double thermal_conductivity = 149; // 149
		const double heat_flow = 1;
		const double convective_heat = 1;
		// set 20 to middle element
		const double intensity_of_heat_source = 100;// i % 2 ? 50 : 0;
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

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() const {
	return index_to_coord_map_;
}

VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}
