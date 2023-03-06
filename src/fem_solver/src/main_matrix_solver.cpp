#include "main_matrix_solver.hpp"

#include <iostream>
#include <iomanip>

void CustomPrintMatrix(const SparceMatrix& matrix, const std::string& matrix_name) {
	std::cout << "Matrix: " << matrix_name << std::endl;
	std::cout << std::setprecision(3);
	for (size_t i = 0; i < matrix.size1(); ++i) {
		for (size_t j = 0; j < matrix.size2(); ++j)
			std::cout << matrix(i, j) << "\t";
		std::cout << std::endl;
	}
	std::cout << "===END OF MATRIX==========" << std::endl;
}

void MatrixEquation::ApplyKnownTemps() {
	assert(known_temp_and_indexes_.size() 
		   && "if we don't know any temps, sistem has infinite number of solutions");
	for (const auto& temp_and_index : known_temp_and_indexes_) {
		const auto index = temp_and_index.second;
		const auto temperature = temp_and_index.first;
		// TODO use subrange
		assert(coeficients_.size1() == coeficients_.size2());
		for (size_t i = 0; i < coeficients_.size1(); ++i) {
			const ValType new_result = result_(i, 0)
				- coeficients_(i, index) * temperature;
			result_(i, 0) = new_result;
		}
		for (size_t i = 0; i < coeficients_.size1(); ++i) {
			coeficients_(i, index) = 0;
			coeficients_(index, i) = 0;
		}
		coeficients_(index, index) = 1;
		result_(index, 0) = temperature;
	}
}

const SolverHeatmap& MatrixEquation::SolveBoostLuFactorisation() {
	assert(!already_solved_ && "Can be solved only one time");
	//CustomPrintMatrix(coeficients_, "MAIN MATRIX coeficients BEFORE");
	//CustomPrintMatrix(result_, "MAIN MATRIX result BEFORE");
	ApplyKnownTemps();
	//CustomPrintMatrix(coeficients_, "MAIN MATRIX coeficients after");
	//CustomPrintMatrix(result_, "MAIN MATRIX result after");
	already_solved_ = true;
	SparceMatrix Afactorized = coeficients_;
	//SparceMatrix Ainv = boost::numeric::ublas::identity_matrix<float>(A.size1());
	boost::numeric::ublas::vector<double> x_boost(Afactorized.size1(), 1);
	boost::numeric::ublas::permutation_matrix<size_t> pm(Afactorized.size1());
	SparceMatrix result = boost::numeric::ublas::identity_matrix<float>(coeficients_.size1());
	// вырожденная
	int singular = boost::numeric::ublas::lu_factorize(Afactorized, pm);
	if (singular) {
		throw std::runtime_error("[LinearSolver<LU>::solve()] A is singular.");
	}
	result = result_;
	boost::numeric::ublas::lu_substitute(Afactorized, pm, result);
	//CustomPrintMatrix(result, "heatmap");
	heatmap_ = SolverHeatmap(result);
	return heatmap_;
}

const SolverHeatmap& MatrixEquation::SolveCholeskyFactorisation() {
	return heatmap_;
}
