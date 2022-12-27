#pragma once

#include "solver_db.hpp"

class MatrixAgregator {
 public:
	MatrixAgregator()  = default;
	virtual ~MatrixAgregator() = default;
	MainMatrix AgregateFsResultMatrix(const FsMatrixVec& db);
};
