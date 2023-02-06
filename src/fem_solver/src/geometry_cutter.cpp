#include "geometry_cutter.hpp"
#include "variance_solver.hpp"

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep) {
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
		std::array<VerticeIndexes::VerticeIndex, 4> indexes = {2,3,4,5};
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

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() const {
	return index_to_coord_map_;
}

VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}
