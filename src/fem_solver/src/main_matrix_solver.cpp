#include "main_matrix_solver.hpp"

#include <iostream>

void CustomPrintMatrix(const Matrix& matrix, const std::string& matrix_name) {
	std::cout << "Matrix: " << matrix_name << std::endl;
	for (unsigned i = 0; i < matrix.size1(); ++i) {
		for (unsigned j = 0; j < matrix.size2(); ++j)
			std::cout << matrix(i, j) << "\t";
		std::cout << std::endl;
	}
	std::cout << "===END OF MATRIX==========" << std::endl;
}

const SolverHeatmap& MatrixEquation::Solve() {
	assert(!already_solved_ && "Can be solved only one time");
	already_solved_ = true;
	Matrix& A = coeficients_;
	Matrix& y = result_;
	//CustomPrintMatrix(y, "input result");
	//CustomPrintMatrix(A, "input coeficients");
	Matrix Afactorized = A;
	Matrix Ainv = boost::numeric::ublas::identity_matrix<float>(A.size1());
	boost::numeric::ublas::vector<double> x_boost(Afactorized.size1(), 1);
	boost::numeric::ublas::permutation_matrix<size_t> pm(Afactorized.size1());
	Matrix result = boost::numeric::ublas::identity_matrix<float>(A.size1());
	// вырожденная
	int singular = boost::numeric::ublas::lu_factorize(Afactorized, pm);
	if (singular) {
		throw std::runtime_error("[LinearSolver<LU>::solve()] A is singular.");
	}
	result = y;
	boost::numeric::ublas::lu_substitute(Afactorized, pm, result);
	//CustomPrintMatrix(result, "heatmap");
	heatmap_ = SolverHeatmap(result);
	return heatmap_;
}
