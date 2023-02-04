#pragma once

#include "solver_db.hpp"

void CustomPrintMatrix(const Matrix& matrix, const std::string& matrix_name);

/// <summary>
/// 
/// </summary>
class MatrixEquation {
 public:
  using Result = Matrix;
  using Coeficients = Matrix;
  MatrixEquation(size_t size_1, size_t size_2) 
		: size_1_(size_1), size_2_(size_2_), coeficients_(size_1, size_2),
		result_(size_1, 1) {};
  void AddResult(const size_t index, const ValType value) {
	result_(index, 0) = value;
  }
  void AddCoeficient(const size_t index_1, const size_t index_2, const ValType value) {
	coeficients_(index_1, index_2) += value;
  }
  void SetHeatmap(const SolverHeatmap& heatmap) {
		heatmap_ = heatmap;
  }
  const SolverHeatmap& Solve();
  const SolverHeatmap& heatmap() const {
	assert(already_solved_);
	return heatmap_;
  }
 private:
  size_t size_1_ = 0;
  size_t size_2_ = 0;
  Coeficients coeficients_;
  SolverHeatmap heatmap_;
  Result result_;
  bool already_solved_ = false;
};

