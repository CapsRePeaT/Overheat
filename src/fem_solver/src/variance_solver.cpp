#include "variance_solver.hpp"

#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace bnu = boost::numeric::ublas;

void VarianceTetraeder::AddElementContribution(MatrixEquation& matrix) const {
	// flux это F, вписываем в основное уравнение (переходим из локальных индексов в глобальные)
    // thermal_conductivity_matrix это К, вписываем в основное уравнение (переходим из локальных индексов в глобальные)
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j)
			matrix.AddCoeficient(indexes()[i], indexes()[i], thermal_conductivity_matrix_(i, j));
		matrix.AddResult(indexes()[i], flux_(i, 0));
	}
	std::cout << "Add element contribution to matrix" << std::endl;
}

double CalcDeterminant(boost::numeric::ublas::matrix<double> matrix) {
	assert(matrix.size1() == matrix.size2() 
		   && "Can only calculate the determinant of square matrices");
	boost::numeric::ublas::permutation_matrix<std::size_t> pivots(matrix.size1());
	// is singular
	if (boost::numeric::ublas::lu_factorize(matrix, pivots))
		return 0;
	double determinant = 1;
	for (std::size_t i = 0; i != pivots.size(); ++i) {
		if (pivots(i) != i)
			determinant *= -1;
		determinant *= matrix(i, i);
	}
	return determinant;
}

// we need determinants of small matrices only, to inprove comp speed aim only for them
double CalcDeterminant3x3(const boost::numeric::ublas::matrix<double>& m) {
		assert(m.size1() == 3 && m.size2() == 3 && "Only for 3x3 matrices");
		const double a = m(0, 0);
		const double b = m(0, 1);
		const double c = m(0, 2);
		const double d = m(1, 0);
		const double e = m(1, 1);
		const double f = m(1, 2);
		const double g = m(2, 0); 
		const double h = m(2, 1);
		const double k = m(2, 2);
		const double determinant
			= (a * ((e * k) - (f * h)))
			- (b * ((k * d) - (f * g)))
			+ (c * ((d * h) - (e * g)));
		return determinant;
}

const std::array<Matrix, 4>& VarianceTetraeder::matrixes_by_side() {
// convective heat computation
// 
// 0 1 2 3 vertice index  
// 2 1 1 1 source matrix
// 1 2 1 1
// 1 1 2 1
// 1 1 1 2
//
// if for side vert is unused, replace it's row and column with 0.
// example: side 0 , vert 3 not present.
// result matrix:
// 2 1 1 0 
// 1 2 1 0
// 1 1 2 0
// 0 0 0 0
//
	static Matrix matrix_side_template(4, 4, 1);
	matrix_side_template += boost::numeric::ublas::identity_matrix<float>(4);
	auto RemoveColumnAndRow = [](const size_t column_index, auto matrix) {
		assert(matrix.size1() == matrix.size2());
		for (size_t i = 0; i < matrix.size1(); ++i) {
			matrix(i, column_index) = 0;
			matrix(column_index, i) = 0;
		}
		return matrix;
	};
	static std::array<Matrix, 4> matrixes_by_side = [&RemoveColumnAndRow] {
		std::array<Matrix, 4> result;
		for (size_t i = 0; i < 4; ++i) {
			result[i] = RemoveColumnAndRow(i, matrix_side_template);
		}
		return result;
	} ();
	return matrixes_by_side;
}

VarianceTetraeder::VarianceTetraeder(double thermal_conductivity,
	double ambient_temperature,
	double heat_flow,
	double intensity_of_heat_source,
	double convective_heat_coef,
	Indexes inp_indexes,
	std::array<bool, 4> convective_presense_per_side,
	std::array<bool, 4> heat_flow_presense_per_side,
	const VerticeIndexes& index_to_coord_map
) :
	SolverTetraeder(thermal_conductivity, ambient_temperature, heat_flow,
		intensity_of_heat_source, convective_heat_coef, inp_indexes)
	, index_to_coord_map_(index_to_coord_map) {
	Matrix coordinates_and_coef(4,4);
	for (size_t i = 0; i < 4; ++i) {
		coordinates_and_coef(i, 0) = 1;
		for (size_t j = 0; j < 3; ++j)
			coordinates_and_coef(i, 1 + j) = index_to_coord_map_.GetCoords(indexes()[i]).coords[j];
	}
	// CustomPrintMatrix(coordinates_and_coef, "coordinates_and_coef");
	coord_det = std::abs(CalcDeterminant(coordinates_and_coef));
	const auto co_factor = ComputeCoFactor(coordinates_and_coef);
	const auto side_square = ComputeSideSquare();
	ComputeThermalConductivityMatrix(co_factor, heat_flow_presense_per_side, side_square);
	ComputeFlux(convective_presense_per_side, heat_flow_presense_per_side, side_square);
	CustomPrintMatrix(thermal_conductivity_matrix_, "thermal_conductivity_matrix_");
	CustomPrintMatrix(flux_, "flux_");
}

std::array<double, 4> VarianceTetraeder::ComputeSideSquare() {
	std::array<double, 4> result;
	auto AreaByHeron = [](const auto a, const auto b, const auto c) {
		const auto s = (a + b + c) / 2;
		return sqrt(s * (s - a) * (s - b) * (s - c));
	};
	auto Distance = [](const auto p1, const auto p2) {
		return sqrt(pow(p2.coords[X] - p1.coords[X], 2) +
			        pow(p2.coords[Y] - p1.coords[Y], 2) +
			        pow(p2.coords[Z] - p1.coords[Z], 2) /* *1.0 */);
	};
	auto ComputeDistance = [&AreaByHeron, &Distance]
		(const auto p1, const auto p2, const auto p3) {
		const auto a = Distance(p1, p2);
		const auto b = Distance(p2, p3);
		const auto c = Distance(p3, p1);
		return AreaByHeron(a, b, c);
	};
	result[0] = ComputeDistance(index_to_coord_map_.GetCoords(indexes()[0]),
								index_to_coord_map_.GetCoords(indexes()[1]),
								index_to_coord_map_.GetCoords(indexes()[2]));
	result[1] = ComputeDistance(index_to_coord_map_.GetCoords(indexes()[0]),
								index_to_coord_map_.GetCoords(indexes()[1]),
								index_to_coord_map_.GetCoords(indexes()[3]));
	result[2] = ComputeDistance(index_to_coord_map_.GetCoords(indexes()[0]),
								index_to_coord_map_.GetCoords(indexes()[2]),
								index_to_coord_map_.GetCoords(indexes()[3]));
	result[3] = ComputeDistance(index_to_coord_map_.GetCoords(indexes()[1]),
								index_to_coord_map_.GetCoords(indexes()[2]),
								index_to_coord_map_.GetCoords(indexes()[3]));
	return result;
}

Matrix VarianceTetraeder::ComputeCoFactor(const Matrix& coordinates_and_coef) {
	// find co-factor matrix project
    // https://www.cuemath.com/algebra/cofactor-matrix/
	Matrix co_factor(4, 4);
	const auto size_1 = coordinates_and_coef.size1();
	const auto size_2 = coordinates_and_coef.size2();
	int sign_iterator = -1; // first element should be positive
	auto CoordToStr = [](auto size_1, auto size_2) {
		std::string coord;
		coord += std::to_string(size_1);
		coord += ", ";
		coord += std::to_string(size_2);
		return coord;
	};
	for (size_t i = 0; i < size_1; ++i) {
		for (size_t j = 0; j < size_2; ++j) {
			Matrix minor_matrix(3, 3);
			// example:
			// 
			//      j----> (size_2)
			//    i 0 1 2
			//    | 3 X 5
			//    | 6 7 8 
			//    *
			// (size_1)
			// 
			// determinant combines from up to 4 submatrixes
			// top left from x  (0 in example)
			if (i > 0 && j > 0)
				bnu::subrange(minor_matrix, 0, i, 0, j) = bnu::subrange(coordinates_and_coef, 0, i, 0, j);
			// top right from x (2 in example)
			if (i > 0 && j < size_2 - 1)
				bnu::subrange(minor_matrix, 0, i, j, 3) = bnu::subrange(coordinates_and_coef, 0, i, j + 1, size_2);
			// bot left from x  (6 in example)
			if (i < size_1 - 1 && j > 0)
				bnu::subrange(minor_matrix, i, 3, 0, j) = bnu::subrange(coordinates_and_coef, i + 1, size_1, 0, j);
			// bot right from x (8 in example)
			if (i < size_1 - 1 > 0 && j < size_2 - 1)
				bnu::subrange(minor_matrix, i, 3, j, 3) = bnu::subrange(coordinates_and_coef, i + 1, size_1, j + 1, size_2);
			// https://en.wikipedia.org/wiki/Minor_(linear_algebra)
			const auto minor = CalcDeterminant3x3(minor_matrix);
			//CustomPrintMatrix(determinant_matrix, CoordToStr(i,j));
			//std::cout << "determinant " << CoordToStr(i, j) << ": " << determinant << std::endl;
			sign_iterator = (i + j) % 2 ? -1 : 1;
			co_factor(i, j) = sign_iterator * minor;
		}
	}
	//CustomPrintMatrix(co_factor, "co_factor");
	return co_factor;
}

void VarianceTetraeder::ComputeThermalConductivityMatrix(const Matrix& co_factor,
	const std::array<bool, 4>& convective_presense_per_side,
	const std::array<double, 4>& side_square) {
	Matrix thermal_conductivity_coef_matrix = boost::numeric::ublas::identity_matrix<float>(3);
	thermal_conductivity_coef_matrix *= thermal_conductivity();
	//CustomPrintMatrix(thermal_conductivity_coef_matrix, "thermal_conductivity_coef_matrix");

	const auto cutted_co_factor = bnu::subrange(co_factor, 0, co_factor.size1(), 1, co_factor.size2());
	const auto cutted_co_factor_t = bnu::trans(cutted_co_factor);
	//CustomPrintMatrix(cutted_co_factor, "cutted_co_factor");
	thermal_conductivity_matrix_ = bnu::prod(cutted_co_factor, thermal_conductivity_coef_matrix);
	thermal_conductivity_matrix_
		= bnu::prod(thermal_conductivity_matrix_, cutted_co_factor_t);
	thermal_conductivity_matrix_ = thermal_conductivity_matrix_ / (6 * coord_det);
	for (size_t i = 0; i < convective_presense_per_side.size(); ++i) {
		if (convective_presense_per_side[i]) {
			thermal_conductivity_matrix_ += matrixes_by_side()[i] * convective_heat_coef()
				* side_square[i] / 12;
		}
	}
	//CustomPrintMatrix(thermal_conductivity_matrix_, "thermal_conductivity_matrix");
}

void VarianceTetraeder::ComputeFlux(const std::array<bool, 4>& convective_presense_per_side,
	                                const std::array<bool, 4>& heat_flow_presense_per_side,
	                                const std::array<double, 4>& side_square) {
	flux_ = Matrix(4, 1, 1);
	flux_ *= intensity_of_heat_source() * coord_det / (4 * 6);
	// heat flow computation (as part of flux vector computation) 
	Matrix zero_at_missing_vert(4, 1, 1);
	for (size_t i = 0; i < heat_flow_presense_per_side.size(); ++i) {
		if (heat_flow_presense_per_side[i]) {
			zero_at_missing_vert(i, 0) = 0;
			flux_ += zero_at_missing_vert * heat_flow() * side_square[i] / 3;
			zero_at_missing_vert(i, 0) = 1;
		}
	}
	for (size_t i = 0; i < convective_presense_per_side.size(); ++i) {
		if (convective_presense_per_side[i]) {
			zero_at_missing_vert(i, 0) = 0;
			flux_ += zero_at_missing_vert * ambient_temperature()
				* convective_heat_coef() * side_square[i] / 3;
			zero_at_missing_vert(i, 0) = 1;
		}
	}
}

// перемножение матриц
	//Matrix test = bnu::prod(coordinates_and_coef, co_factor);
	//CustomPrintMatrix(test, "prod");
	//// транспонирование матрицы 
	//test.resize(4, 3);
	//CustomPrintMatrix(test, "resize");
	//test = bnu::trans(test);
	//CustomPrintMatrix(test, "trans");
