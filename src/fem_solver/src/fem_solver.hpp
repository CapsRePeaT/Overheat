#pragma once

#include "geometry_cutter.hpp"
#include "solver_db.hpp"

#include "../include/i_fem_solver.hpp"

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"

class FemSolver : public IFemSolver {
 public:
	FemSolver() = default;
	virtual void Solve(FileRepresentation& file_rep, bool test_flow = false) final;
	virtual ~FemSolver() = default;
};
