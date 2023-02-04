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
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 3; ++k)
				points[j].coords[k] = i + j + k;
			indexes[j] = GetVerticeIndexes().AddVertice(points[j]);
		}
		result.AddElement(new VarianceTetraeder(
			0, 0, 0, 0, 0, indexes, GetVerticeIndexes()));
	}
	return result;
}

const VerticeIndexes& GeometryCutter::GetVerticeIndexes() const {
	return index_to_coord_map_;
}

VerticeIndexes& GeometryCutter::GetVerticeIndexes() {
	return index_to_coord_map_;
}
