#include "geometry_cutter.hpp"
#include "variance_solver.hpp"

FsDatapack GeometryCutter::PrepareGeometry(FileRepresentation& file_rep) {
	std::cout << "triangulating geometry..." << std::endl;
	std::cout << "converting geometry to db..." << std::endl;
	FsDatapack result;
	for (int i = 0; i < 3; ++i) {
		std::array<VerticeIndexes::VerticeIndex, 4> dummy_indexes;
		for (int i = 0; i < 4; ++i) {
			dummy_indexes[i] = 0;
		}
		result.AddElement(new VarianceTetraeder(
			0, 0, 0, 0, 0, dummy_indexes, GetVerticeIndexes()));
	}
	return result;
}

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}
