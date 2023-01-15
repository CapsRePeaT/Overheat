#pragma once

#include <array>

#include "solver_db.hpp"

class SolverShape {
 public:
	SolverShape() = default;
	virtual ~SolverShape() = default;
	virtual void AddElementContribution(MatrixEquation& matrix) const = 0;
 private:
};

class SolverTetraeder : public SolverShape {
 public:
	 using VerticeIndexAndCoord 
		 = std::pair<VerticeIndexes::VerticeIndex, VerticeIndexes::Point3D>;
	 SolverTetraeder(double thermal_conductivity,
		 double ambient_temperature,
		 double heat_flow,
		 double intensity_of_heat_source,
		 double convective_heat,
		 std::array<VerticeIndexes::VerticeIndex, 4> indexes) : SolverShape(),
		 thermal_conductivity_(thermal_conductivity),
		 ambient_temperature_(ambient_temperature),
		 heat_flow_(heat_flow), intensity_of_heat_source_(intensity_of_heat_source),
		 convective_heat_(convective_heat), indexes_(indexes) {}
	~SolverTetraeder() = default;
 private:
	 double thermal_conductivity_;
	 double ambient_temperature_;
	 double heat_flow_;
	 double intensity_of_heat_source_;
	 double convective_heat_;
	 std::array<VerticeIndexes::VerticeIndex, 4> indexes_;
};

