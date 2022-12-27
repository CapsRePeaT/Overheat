#pragma once

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"

class IFemSolver {
 public:
	IFemSolver()  = default;
	// main event loop
	virtual void Solve(FileRepresentation& file_rep) = 0;

	virtual ~IFemSolver() = default;
};
