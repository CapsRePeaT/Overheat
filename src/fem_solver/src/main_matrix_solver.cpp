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

const SolverHeatmap& MatrixEquation::Solve() {
	assert(!already_solved_ && "Can be solved only one time");
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
