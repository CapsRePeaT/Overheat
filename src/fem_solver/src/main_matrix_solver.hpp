#pragma once

#include "solver_db.hpp"

void CustomPrintMatrix(const SparceMatrix& matrix, const std::string& matrix_name);

class MatrixEquation {
 public:
  using Result = SparceMatrix;
  using Coeficients = SparceMatrix;
  using TempAndIndex = std::pair<double, size_t>;
  MatrixEquation(const size_t size) 
		: size_(size), coeficients_(size, size),
		result_(size, 1) {};
  void AddResult(const size_t index, const ValType value) {
	assert(index < size_);
	result_(index, 0) = value; //treat matrix as vector
  }
  void AddCoeficient(const size_t index_1, const size_t index_2, const ValType value) {
	assert(index_1 < size_&& index_2 < size_);
	coeficients_(index_1, index_2) += value;
  }
  void SetHeatmap(const SolverHeatmap& heatmap) {
	heatmap_ = heatmap;
  }
  void SetMaxSize(const size_t size) {
	coeficients_.resize(size, size);
	result_.resize(size, 1);
  }
	const SolverHeatmap& heatmap() const {
		assert(already_solved_);
		return heatmap_;
	}
	void AddKnownTempAndIndex(const TempAndIndex& temp_and_index) {
		known_temp_and_indexes_.emplace_back(temp_and_index);
	}
	void set_known_temp_and_indexes(
		const std::vector<TempAndIndex>& known_temp_and_indexes) {
		known_temp_and_indexes_ = known_temp_and_indexes;
	}
  // solvers
  const SolverHeatmap& SolveBoostLuFactorisation();
  const SolverHeatmap& SolveHYPRE();
  // runs many solvers in a row, may not work
  const SolverHeatmap& SolveHYPRETest();
 private:
  void ApplyKnownTemps();
  size_t size_ = DefaultMatrixSize;
  Coeficients coeficients_;
  SolverHeatmap heatmap_;
  Result result_;
  std::vector<TempAndIndex> known_temp_and_indexes_;
  bool already_solved_ = false;
};

