#pragma once

#include "geometry_cutter.hpp"
#include "solver_db.hpp"

#include "../include/i_fem_solver.hpp"

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"


class FemSolver : public IFemSolver {
 public:
	FemSolver() = default;
	/// <summary>
	/// Solves heat thransfer equation
	/// </summary>
	/// <param name="file_rep">
	/// Database with list of shapes, corner conditions and fisical constants of materials
	/// </param>
	/// List of solver options
	/// <param name="setup"></param>
	virtual void Solve(FileRepresentation& file_rep, SolverSetup setup) final;
	virtual ~FemSolver() = default;
};
