#pragma once

#include <vector>
#include <deque>
#include <iostream>

// span needed because of the issues with _ITERATOR_DEBUG_LEVEL
// see https://github.com/boostorg/ublas/issues/77
#include <span>
#include "boost/numeric/ublas/matrix_expression.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_sparse.hpp"
#include "boost/numeric/ublas/lu.hpp"

#include "../../common/include/common.h"

class SolverShape;

class FsDatapack {
 public:
  bool NextElement(SolverShape*& element);
  void AddElement(SolverShape* element);
  FsDatapack() = default;
 private:
  std::deque<SolverShape*> elements_;
};

class VerticeIndexes {
 public:
  // FIXME
  using Point3D = double;
  using VerticeIndex = size_t;
  VerticeIndex AddVertice(Point3D point);
  Point3D GetCoords(VerticeIndex index);
  // needed for heatmap interpolation
  std::array<VerticeIndex, 4> GetConvexHull(Point3D point);
 private:
  std::vector<Point3D> coords_;
	// add search tree
};

class FsResultMatrix {
 public:
  FsResultMatrix() = default;
};
using FsMatrixVec = std::vector<FsResultMatrix>;

using ValType = float;
using Matrix = boost::numeric::ublas::compressed_matrix<ValType>;

class SolverHeatmap {
public:
	using Temperature = ValType;
	SolverHeatmap() = default;
	SolverHeatmap(const Matrix& matrix) {
		FillData(matrix);
	}
	void FillData(const Matrix& matrix) {
		assert(temperatures_.empty() && "Heatmap not adapted for rewriting");
		for (unsigned i = 0; i < matrix.size1(); ++i)
			temperatures_.push_back(matrix(i, 0));
	}
	void Print() const;
private:
	size_t max_index_ = 0;
	// element index equal to position index
	std::deque<Temperature> temperatures_;
};




