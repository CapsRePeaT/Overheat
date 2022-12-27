#pragma once

#include "solver_db.hpp"

class Solver {
 public:
	Solver()  = default;
	virtual ~Solver() = default;
	virtual FsMatrixVec ComputeMatrix(const FsDatapack& db) = 0;
};

class VarianceSolver {
 public:
	VarianceSolver()                                      = default;
	virtual ~VarianceSolver()                             = default;
	virtual FsMatrixVec ComputeMatrix(const FsDatapack& db) final;
};
