#pragma once

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"

class IFemSolver {
 public:
	IFemSolver()  = default;
	virtual void Solve() = 0;

	virtual ~IFemSolver() = default;
};
