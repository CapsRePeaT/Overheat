#pragma once

#include "../include/i_fem_solver.hpp"

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"

class FemSolver : public IFemSolver {
 public:
	virtual void Solve() final {
		std::cout << "solving heat transfer..." << std::endl;
	};

	virtual ~FemSolver() = default;
};
