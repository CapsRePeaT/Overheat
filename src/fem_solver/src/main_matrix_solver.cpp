#include "main_matrix_solver.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>

#include <boost/numeric/ublas/matrix_proxy.hpp>

#include "_hypre_IJ_mv.h"
#include "HYPRE_parcsr_ls.h" // ILU, MGR
#include "HYPRE_parcsr_mv.h"

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

namespace bnu = boost::numeric::ublas;

void MatrixEquation::ApplyKnownTemps() {
	auto timer_start = std::chrono::high_resolution_clock::now();
	assert(known_temp_and_indexes_.size() 
		   && "if we don't know any temps, sistem has infinite number of solutions");
	Matrix horisontal_zeros(1, coeficients_.size2(), 0);
	Matrix vertical_zeros(coeficients_.size1(), 1, 0);
	int counter = 0;
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
		bnu::subrange(coeficients_, 0, coeficients_.size1(), index, index + 1) = bnu::subrange(vertical_zeros, 0, coeficients_.size1(), 0, 1);
		bnu::subrange(coeficients_, index, index + 1, 0, coeficients_.size2()) = bnu::subrange(horisontal_zeros, 0, 1, 0, coeficients_.size1());
		//for (size_t i = 0; i < coeficients_.size1(); ++i) {
		//	coeficients_(i, index) = 0;
		//	coeficients_(index, i) = 0;
		//}
		coeficients_(index, index) = 1;
		result_(index, 0) = temperature;
		++counter;
	}
	auto timer_end = std::chrono::high_resolution_clock::now();
	std::cout << "ApplyKnownTemps fineshed, it took "
		<< std::chrono::duration_cast<std::chrono::seconds>(timer_end - timer_start)
		<< " seconds." << std::endl;
}

const SolverHeatmap& MatrixEquation::SolveBoostLuFactorisation() {
	assert(!already_solved_ && "Can be solved only one time");
	CustomPrintMatrix(coeficients_, "MAIN MATRIX coeficients BEFORE");
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

const SolverHeatmap& MatrixEquation::SolveHYPRE() {
	ApplyKnownTemps();
	already_solved_ = true;
	// setup matrix
	const int nrows = coeficients_.size1();
	const int ncols = coeficients_.size2();
	const int num_of_elements = ncols * nrows;

	static bool init_needed = true;
	if (init_needed) {
		MPI_Init(NULL, NULL);
		HYPRE_Init();
		init_needed = false;
	}
	

	HYPRE_IJMatrix ij_matrix; //matrix
	{
		std::vector<int> ncols_vec(nrows, ncols);
		std::vector<int> rows(nrows);
		std::vector<int> cols(num_of_elements);
		std::vector<double> values(num_of_elements);
		for (int i = 0; i < nrows; ++i) {
			rows[i] = i;
			for (int j = 0; j < ncols; ++j) {
				const int index = i * ncols + j;
				cols[index] = j;
				values[index] = coeficients_(i, j);
			}
		}
		HYPRE_IJMatrixCreate(MPI_COMM_WORLD,
			0 /*ilower*/, nrows /*iupper*/,
			0 /*jlower*/, ncols /*jupper*/, &ij_matrix);
		HYPRE_IJMatrixSetObjectType(ij_matrix, HYPRE_PARCSR);
		HYPRE_IJMatrixInitialize(ij_matrix);
		HYPRE_IJMatrixSetValues(ij_matrix, nrows,
			ncols_vec.data(), rows.data(),
			cols.data(), values.data());
		HYPRE_IJMatrixAssemble(ij_matrix);
		//HYPRE_IJMatrixPrint(ij_matrix, "Matrix_debug.txt");
	}
	HYPRE_IJVector ij_vector_b; // right hand side
	{
		int nvalues = result_.size1();
		std::vector<int> indices(nvalues);
		std::vector<double> values(nvalues);
		for (int i = 0; i < ncols; ++i) {
			indices[i] = i;
			values[i] = result_(i, 0);
		}
		HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, result_.size1() - 1, &ij_vector_b);
		HYPRE_IJVectorSetObjectType(ij_vector_b, HYPRE_PARCSR);
		HYPRE_IJVectorInitialize(ij_vector_b);
		HYPRE_IJVectorSetValues(ij_vector_b, nvalues, indices.data(), values.data());
		HYPRE_IJVectorAssemble(ij_vector_b);
		//HYPRE_IJVectorPrint(ij_vector_b, "Vector_debug_b.txt");
	}
	HYPRE_IJVector ij_vector_x; // solution
	{
		int nvalues = result_.size1();
		std::vector<int> indices(nvalues);
		std::vector<double> values(nvalues, 0);
		for (int i = 0; i < ncols; ++i) {
			indices[i] = i;
		}
		HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, result_.size1() - 1, &ij_vector_x);
		HYPRE_IJVectorSetObjectType(ij_vector_x, HYPRE_PARCSR);
		HYPRE_IJVectorInitialize(ij_vector_x);
		HYPRE_IJVectorSetValues(ij_vector_x, nvalues, indices.data(), values.data());
		HYPRE_IJVectorAssemble(ij_vector_x);
		//HYPRE_IJVectorPrint(ij_vector_x, "Vector_debug_x.txt");
	}
	std::cout << "solver preparations finished" << std::endl;
	HYPRE_Solver solver;
	HYPRE_ParCSRHybridCreate(&solver);
	//https://oomph-lib.github.io/oomph-lib/doc/the_data_structure/html/hypre__solver_8cc_source.html
	HYPRE_ParCSRMatrix hypre_par_matrix;
	//HYPRE_ParCSRMatrixCreate()
	HYPRE_IJMatrixGetObject(ij_matrix, (void**)&hypre_par_matrix); // durty hack
	HYPRE_ParVector hypre_par_vector_b;
	HYPRE_IJVectorGetObject(ij_vector_b, (void**)&hypre_par_vector_b);
	HYPRE_ParVector hypre_par_vector_x;
	HYPRE_IJVectorGetObject(ij_vector_x, (void**)&hypre_par_vector_x);
	HYPRE_ParCSRHybridSetup(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
	HYPRE_ParCSRHybridSolve(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
	// geting data
	int nvalues = result_.size1();
	std::vector<int> indices;
	indices.reserve(nvalues);
	for (int i = 0; i < ncols; ++i) {
		indices.push_back(i);
	}
	std::vector<double> values(nvalues, 0);
	hypre_ParVectorGetValues(hypre_par_vector_x, nvalues, indices.data(), values.data());
	HYPRE_ParVectorPrint(hypre_par_vector_x, "result_real_hybrid.txt");
	HYPRE_ParCSRHybridDestroy(solver);
	auto& raw_heatmap = heatmap_.data();
	assert(raw_heatmap.size() == 0);
	for (int i = 0; i < ncols; ++i) {
		raw_heatmap.push_back(values[i]);
	}
	return heatmap_;
}

const SolverHeatmap& MatrixEquation::SolveHYPRETest() {
	/* Initialize MPI */
	int myid, num_procs;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	HYPRE_Init();
	HYPRE_IJMatrix ij_matrix; //matrix
	{
		int  nrows = 3;
		int  ncols[3] = {3, 3, 3};
		int  rows[3] = {0, 1, 2};
		int  cols[9] = {0, 1, 2, 
			            0, 1, 2, 
			            0, 1, 2};
		double values[9] = {2, 1, 1, 
			                1, -1, 0, 
			                3, -1, 2};
		HYPRE_IJMatrixCreate(MPI_COMM_WORLD,
			0 /*ilower*/, 3 /*iupper*/,
			0 /*jlower*/, 3 /*jupper*/, &ij_matrix);
		HYPRE_IJMatrixSetObjectType(ij_matrix, HYPRE_PARCSR);
		HYPRE_IJMatrixInitialize(ij_matrix);
		HYPRE_IJMatrixSetValues(ij_matrix, nrows, ncols, rows, cols, values);
		HYPRE_IJMatrixAssemble(ij_matrix);
		HYPRE_IJMatrixPrint(ij_matrix, "Matrix_debug.txt");
	}
	HYPRE_IJVector ij_vector_b; // right hand side
	{
		int nvalues = 3;
		int indices[3] = {0, 1, 2};
		double values[3] = {2, -2, 2};
		HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, 2, &ij_vector_b);
		HYPRE_IJVectorSetObjectType(ij_vector_b, HYPRE_PARCSR);
		HYPRE_IJVectorInitialize(ij_vector_b);
		HYPRE_IJVectorSetValues(ij_vector_b, nvalues, indices, values);
		HYPRE_IJVectorAssemble(ij_vector_b);
		HYPRE_IJVectorPrint(ij_vector_b, "Vector_debug_b.txt");

	}
	HYPRE_IJVector ij_vector_x; // solution
	{
		int nvalues = 3;
		int indices[3] = { 0, 1, 2 };
		double values[3] = { 0, 0, 0 };
		HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, 2, &ij_vector_x);
		HYPRE_IJVectorSetObjectType(ij_vector_x, HYPRE_PARCSR);
		HYPRE_IJVectorInitialize(ij_vector_x);
		HYPRE_IJVectorSetValues(ij_vector_x, nvalues, indices, values);
		HYPRE_IJVectorAssemble(ij_vector_x);
		HYPRE_IJVectorPrint(ij_vector_x, "Vector_debug_x.txt");

	}
	// hybrid(krylov)
	{
		HYPRE_Solver solver;
		HYPRE_ParCSRHybridCreate(&solver);
		//HYPRE_MGRSetCpointsByBlock(solver); // we use default
			//https://oomph-lib.github.io/oomph-lib/doc/the_data_structure/html/hypre__solver_8cc_source.html
		HYPRE_ParCSRMatrix hypre_par_matrix;
		HYPRE_IJMatrixGetObject(ij_matrix, (void**)&hypre_par_matrix);
		HYPRE_ParVector hypre_par_vector_b;
		HYPRE_IJVectorGetObject(ij_vector_b, (void**)&hypre_par_vector_b);
		HYPRE_ParVector hypre_par_vector_x;
		HYPRE_IJVectorGetObject(ij_vector_x, (void**)&hypre_par_vector_x);
		HYPRE_ParCSRHybridSetup(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ParCSRHybridSolve(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ParVectorPrint(hypre_par_vector_x, "result_par_vec_hybrid.txt");
		HYPRE_ParCSRHybridDestroy(solver);
	}
	// ILU
	{
		HYPRE_Solver solver;
		HYPRE_ILUCreate(&solver);
		HYPRE_ILUSetType(solver, 0); //https://hypre.readthedocs.io/en/latest/api-sol-parcsr.html#_CPPv416HYPRE_ILUSetType12HYPRE_Solver9HYPRE_Int
		//https://oomph-lib.github.io/oomph-lib/doc/the_data_structure/html/hypre__solver_8cc_source.html
		HYPRE_ParCSRMatrix hypre_par_matrix;
		HYPRE_IJMatrixGetObject(ij_matrix, (void**)&hypre_par_matrix);
		HYPRE_ParVector hypre_par_vector_b;
		HYPRE_IJVectorGetObject(ij_vector_b, (void**)&hypre_par_vector_b);
		HYPRE_ParVector hypre_par_vector_x;
		HYPRE_IJVectorGetObject(ij_vector_x, (void**)&hypre_par_vector_x);
		HYPRE_ILUSetup(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ILUSolve(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ParVectorPrint(hypre_par_vector_x, "result_par_vec_ilu.txt");
		HYPRE_ILUDestroy(solver);
	}
	// MGR
	{
		HYPRE_Solver solver;
		HYPRE_MGRCreate(&solver);
		//HYPRE_MGRSetCpointsByBlock(solver); // we use default
	    //https://oomph-lib.github.io/oomph-lib/doc/the_data_structure/html/hypre__solver_8cc_source.html
		HYPRE_ParCSRMatrix hypre_par_matrix;
		HYPRE_IJMatrixGetObject(ij_matrix, (void**)&hypre_par_matrix);
		HYPRE_ParVector hypre_par_vector_b;
		HYPRE_IJVectorGetObject(ij_vector_b, (void**)&hypre_par_vector_b);
		HYPRE_ParVector hypre_par_vector_x;
		HYPRE_IJVectorGetObject(ij_vector_x, (void**)&hypre_par_vector_x);
		HYPRE_MGRSetup(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_MGRSolve(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ParVectorPrint(hypre_par_vector_x, "result_par_vec_mgr.txt");
		HYPRE_MGRDestroy(solver);
	}
	// FSAI, based on Cholesky 
	// needed version HYPRE version 2.25.0, as for 13/3/2023 vcpkg supports only 2.23.0
	return heatmap_;
}
