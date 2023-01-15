#pragma once

#include <vector>
#include <deque>
#include <iostream>

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
 private:
  std::vector<Point3D> coords_;
};

class FsResultMatrix {
 public:
  FsResultMatrix() = default;
};
using FsMatrixVec = std::vector<FsResultMatrix>;

class MainMatrix {
 public:
  MainMatrix() = default;
};

class SolverHeatmap {
 public:
  SolverHeatmap() = default;
};
