#pragma once

#include "solver_db.hpp"

class MainMatrixSolver {
 public:
	MainMatrixSolver()  = default;
	virtual ~MainMatrixSolver() = default;
	SolverHeatmap ComputeHeatmap(const MainMatrix& db);
};

