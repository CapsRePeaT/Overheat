#pragma once

#include <vector>
#include <deque>
#include <iostream>

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

//
//    |***|   |****|
//  |*******************|

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
};

class FsResultMatrix {
 public:
  FsResultMatrix() = default;
};
using FsMatrixVec = std::vector<FsResultMatrix>;

class Coeficients {
 public:
	Coeficients() = default;
 private:
	size_t max_index_ = 0;
	// element index equal to position index
	// std::matrix
};

class SolverHeatmap {
 public:
  using Temperature = double;
  SolverHeatmap() = default;
 private:
  size_t max_index_ = 0;
  // element index equal to position index
  std::deque<Temperature> temperatures_;
};

class MatrixEquation {
 public:
  using Result = std::deque<double>;
  MatrixEquation() = default;
 private:
  size_t max_index_ = 0;
  Coeficients coeficients_;
  SolverHeatmap unknown_;
  Result result_;
};
