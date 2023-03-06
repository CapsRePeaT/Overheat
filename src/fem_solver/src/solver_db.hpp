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
	void PrintContent() const {
		std::cout << "shape count: " << elements_.size() << std::endl;
	}
  FsDatapack() = default;
 private:
  std::deque<SolverShape*> elements_;
};

struct Point3D
{
	// TODO: consider using double, cause cinolib uses doubles
	std::array<double, 3> coords;
	friend bool operator<(const Point3D& l, const Point3D& r)
	{
		return std::tie(l.coords[0], l.coords[1], l.coords[2])
			< std::tie(r.coords[0], r.coords[1], r.coords[2]);
	}
};

class VerticeIndexes {
 public:
  using VerticeIndex = size_t;
	VerticeIndex AddVertice(Point3D point) {
		if (coords_map_.find(point) != coords_map_.end()) {
			return coords_map_.at(point);
		} else {
			coords_.push_back(point);
			coords_map_.emplace(coords_.back(), coords_.size() - 1);
			return coords_.size() - 1;
		}
		
	}
	Point3D GetCoords(VerticeIndex index) const {
		return coords_[index];
	}
  // needed for heatmap interpolation
  std::array<VerticeIndex, 4> GetConvexHull(Point3D point);
  size_t MaxIndex() const {
	return coords_.size() - 1;
  }
 private:
  std::map<Point3D, VerticeIndex> coords_map_;
  // FIXME temp solution
  std::vector<Point3D> coords_;
	// add search tree
};

class FsResultMatrix {
 public:
  FsResultMatrix() = default;
};
using FsMatrixVec = std::vector<FsResultMatrix>;

using ValType = float;
using Matrix = boost::numeric::ublas::matrix<ValType>;
using SparceMatrix = boost::numeric::ublas::compressed_matrix<ValType>;

class SolverHeatmap {
public:
	using Temperature = ValType;
	using Temperatures = std::deque<Temperature>;

	SolverHeatmap() = default;
	SolverHeatmap(const SparceMatrix& matrix) {
		FillData(matrix);
	}
	void FillData(const SparceMatrix& matrix) {
		assert(temperatures_.empty() && "Heatmap not adapted for rewriting");
		for (unsigned i = 0; i < matrix.size1(); ++i)
			temperatures_.push_back(matrix(i, 0));
	}
	void Print() const;
	const Temperatures& temperatures() const { 
		return temperatures_;
	}

 private:
	// element index equal to position index
	Temperatures temperatures_;

};




