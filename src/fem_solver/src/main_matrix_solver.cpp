#include "main_matrix_solver.hpp"

#include <iostream>
#include <iomanip>

/* Struct linear solvers header */
#include "HYPRE_struct_ls.h"
//#include "HYPRE_IJ_mv.h"
#include "_hypre_IJ_mv.h"
// tests

#include "HYPRE_parcsr_ls.h" // ILU, MGR



//#include "HYPRE_parcsr_mv.h"
//#include "HYPRE_utilities.h"
//#include "_hypre_utilities.h"
//
//#include "HYPRE.h"
//#include "HYPRE_krylov.h"
//#include "distributed_matrix.h"
//#include "HYPREf.h"
//#include "HYPRE_DistributedMatrixPilutSolver_types.h"


//HYPRE_Solver solver;
///* Create Solver */
//HYPRE_ILUCreate(MPI_COMM_WORLD, &solver);
//HYPRE_MGRCreate(MPI_COMM_WORLD, &solver);
//HYPRE_FSAISetMaxSteps();


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

const SolverHeatmap& MatrixEquation::SolveCholeskyFactorisation() {

	int i, j, myid, num_procs;
	HYPRE_StructGrid     grid;
	HYPRE_StructStencil  stencil;
	HYPRE_StructMatrix   A;
	HYPRE_StructVector   b;
	HYPRE_StructVector   x;
	HYPRE_StructSolver   solver;

	/* Initialize MPI */
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	/* Initialize HYPRE */
	HYPRE_Init();
	//if (num_procs != 2)
	//{
	//	if (myid == 0) { printf("Must run with 2 processors!\n"); }
	//	MPI_Finalize();

	//	return heatmap_;
	//}
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
		int jlower, jupper;
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
		int jlower, jupper;
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


	/* 5. Set up and use a solver (See the Reference Manual for descriptions
	 of all of the options.) */
	{


		///* Create Solver */
		//int HYPRE_SOLVERCreate(MPI_COMM_WORLD, &solver);
		///* Set certain parameters if desired */
		//HYPRE_SOLVERSetTol(solver, 1.e-8);
		///* Set up Solver */
		//HYPRE_SOLVERSetup(solver, A, b, x);
		///* Solve the system */
		//HYPRE_SOLVERSolve(solver, A, b, x);
		///* Destroy the solver */
		//HYPRE_SOLVERDestroy(solver);


		HYPRE_Solver solver;
		/* Create Solver */
		HYPRE_ILUCreate(&solver);
		HYPRE_ILUSetType(solver, 0); //https://hypre.readthedocs.io/en/latest/api-sol-parcsr.html#_CPPv416HYPRE_ILUSetType12HYPRE_Solver9HYPRE_Int
		//HYPRE_ParaSailsBuildIJMatrix(solver, &ij_matrix);
		https://oomph-lib.github.io/oomph-lib/doc/the_data_structure/html/hypre__solver_8cc_source.html
		HYPRE_ParCSRMatrix hypre_par_matrix;
		HYPRE_IJMatrixGetObject(ij_matrix, (void**)&hypre_par_matrix);
		HYPRE_ParVector hypre_par_vector_b;
		HYPRE_IJVectorGetObject(ij_vector_b, (void**)&hypre_par_vector_b);
		HYPRE_ParVector hypre_par_vector_x;
		HYPRE_IJVectorGetObject(ij_vector_x, (void**)&hypre_par_vector_x);
		
		HYPRE_ILUSetup(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ILUSolve(solver, hypre_par_matrix, hypre_par_vector_b, hypre_par_vector_x);
		HYPRE_ParVectorPrint(hypre_par_vector_x, "result_par_vec");
		HYPRE_ILUDestroy(solver);
		//HYPRE_MGRCreate(&solver); // WORKS!
		

		//HYPRE_FSAISetMaxSteps();

		///* Create an empty PCG Struct solver */
		//HYPRE_ILUCreate(MPI_COMM_WORLD, &solver);

		///* Set some parameters */
		//HYPRE_PCGSetTol(solver, 1.0e-06); /* convergence tolerance */
		//HYPRE_PCGSetPrintLevel(solver, 2); /* amount of info. printed */

		///* Setup and solve */
		//HYPRE_PCGSetup(solver, ij_matrix, b, x);
		//HYPRE_PCGSolve(solver, A, b, x);
	}
















	

	

	/* Print GPU info */
	/* HYPRE_PrintDeviceInfo(); */

	/* 1. Set up a grid. Each processor describes the piece
		 of the grid that it owns. */
	{
		/* Create an empty 2D grid object */
		HYPRE_StructGridCreate(MPI_COMM_WORLD, 2, &grid);

		/* Add boxes to the grid */
		if (myid == 0) {
			int ilower[2] = { -3, 1 }, iupper[2] = { -1, 2 };
			HYPRE_StructGridSetExtents(grid, ilower, iupper);
		}

		/* This is a collective call finalizing the grid assembly.
			 The grid is now ``ready to be used'' */
		HYPRE_StructGridAssemble(grid);
	}

	/* 2. Define the discretization stencil */
	{
		/* Create an empty 2D, 5-pt stencil object */
		HYPRE_StructStencilCreate(2, 5, &stencil);
		/* Define the geometry of the stencil. Each represents a
			 relative offset (in the index space). */
		{
			int entry;
			int offsets[5][2] = { {0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
			/* Assign each of the 5 stencil entries */
			for (entry = 0; entry < 5; entry++) {
				HYPRE_StructStencilSetElement(stencil, entry, offsets[entry]);
			}
		}
	}

	/* 3. Set up a Struct Matrix */
	{
		/* Create an empty matrix object */
		HYPRE_StructMatrixCreate(MPI_COMM_WORLD, grid, stencil, &A);
		/* Indicate that the matrix coefficients are ready to be set */
		HYPRE_StructMatrixInitialize(A);
		/* Set the matrix coefficients.  Each processor assigns coefficients
			 for the boxes in the grid that it owns. Note that the coefficients
			 associated with each stencil entry may vary from grid point to grid
			 point if desired.  Here, we first set the same stencil entries for
			 each grid point.  Then we make modifications to grid points near
			 the boundary. */
		if (myid == 0) {
			int ilower[2] = { -3, 1 }, iupper[2] = { -1, 2 };
			int stencil_indices[5] = { 0, 1, 2, 3, 4 }; /* labels for the stencil entries -
													       these correspond to the offsets defined above */
			int nentries = 5;
			int nvalues = 30; /* 6 grid points, each with 5 stencil entries */
			/* double values[30]; OK to use constant-length arrays for CPUs */
			double* values = (double*)malloc(30 * sizeof(double));

			/* We have 6 grid points, each with 5 stencil entries */
			for (i = 0; i < nvalues; i += nentries) {
				values[i] = 4.0;
				for (j = 1; j < nentries; j++) {
					values[i + j] = -1.0;
				}
			}
			HYPRE_StructMatrixSetBoxValues(A, ilower, iupper, nentries,
				stencil_indices, values);
			free(values);
		}
		/* Set the coefficients reaching outside of the boundary to 0 */
		if (myid == 0) {
			/* double values[3]; OK to use constant-length arrays for CPUs */
			double* values = (double*)malloc(3 * sizeof(double));
			for (i = 0; i < 3; i++) {
				values[i] = 0.0;
			}
			{
				/* values below our box */
				int ilower[2] = { -3, 1 }, iupper[2] = { -1, 1 };
				int stencil_indices[1] = { 3 };
				HYPRE_StructMatrixSetBoxValues(A, ilower, iupper, 1,
					stencil_indices, values);
			}
			{
				/* values to the left of our box */
				int ilower[2] = { -3, 1 }, iupper[2] = { -3, 2 };
				int stencil_indices[1] = { 1 };
				HYPRE_StructMatrixSetBoxValues(A, ilower, iupper, 1,
					stencil_indices, values);
			}
			{
				/* values above our box */
				int ilower[2] = { -3, 2 }, iupper[2] = { -1, 2 };
				int stencil_indices[1] = { 4 };
				HYPRE_StructMatrixSetBoxValues(A, ilower, iupper, 1,
					stencil_indices, values);
			}
			free(values);
		}
		/* This is a collective call finalizing the matrix assembly.
			 The matrix is now ``ready to be used'' */
		HYPRE_StructMatrixAssemble(A);
	}

	/* 4. Set up Struct Vectors for b and x.  Each processor sets the vectors
		 corresponding to its boxes. */
	{
		/* Create an empty vector object */
		HYPRE_StructVectorCreate(MPI_COMM_WORLD, grid, &b);
		HYPRE_StructVectorCreate(MPI_COMM_WORLD, grid, &x);

		/* Indicate that the vector coefficients are ready to be set */
		HYPRE_StructVectorInitialize(b);
		HYPRE_StructVectorInitialize(x);

		/* Set the vector coefficients */
		if (myid == 0) {
			int ilower[2] = { -3, 1 }, iupper[2] = { -1, 2 };
			/* double values[6]; OK to use constant-length arrays for CPUs */
			double* values = (double*)malloc(6 * sizeof(double)); /* 6 grid points */

			for (i = 0; i < 6; i++) {
				values[i] = 1.0;
			}
			HYPRE_StructVectorSetBoxValues(b, ilower, iupper, values);

			for (i = 0; i < 6; i++) {
				values[i] = 0.0;
			}
			HYPRE_StructVectorSetBoxValues(x, ilower, iupper, values);
			free(values);
		}

		/* This is a collective call finalizing the vector assembly.
			 The vectors are now ``ready to be used'' */
		HYPRE_StructVectorAssemble(b);
		HYPRE_StructVectorAssemble(x);
	}

	/* 5. Set up and use a solver (See the Reference Manual for descriptions
		 of all of the options.) */
	{
		/* Create an empty PCG Struct solver */
		HYPRE_StructPCGCreate(MPI_COMM_WORLD, &solver);

		/* Set some parameters */
		HYPRE_StructPCGSetTol(solver, 1.0e-06); /* convergence tolerance */
		HYPRE_StructPCGSetPrintLevel(solver, 2); /* amount of info. printed */

		/* Setup and solve */
		HYPRE_StructPCGSetup(solver, A, b, x);
		HYPRE_StructPCGSolve(solver, A, b, x);
	}

	/* Free memory */
	HYPRE_StructGridDestroy(grid);
	HYPRE_StructStencilDestroy(stencil);
	HYPRE_StructMatrixDestroy(A);
	HYPRE_StructVectorDestroy(b);
	HYPRE_StructVectorDestroy(x);
	HYPRE_StructPCGDestroy(solver);

	/* Finalize Hypre */
	HYPRE_Finalize();

	/* Finalize MPI */
	MPI_Finalize();

	return heatmap_;
}
