#pragma once

#include <array>

#include "solver_db.hpp"
#include "main_matrix_solver.hpp"

class SolverShape {
 public:
	SolverShape() = default;
	virtual ~SolverShape() = default;
	virtual void AddElementContribution(MatrixEquation& matrix) const = 0;
 private:
};

// legend:
// () - vertice
// [4] - face
// 
//               *(0)         
//              /|\
//             / | \
//            /  |  \
//           /[2]|[1]\ 
//          /    |    \
//         /    _*(3)  \    
//        /   _/   \_   \      
//       /  _/       \_  \  
//      / _/    [3]    \_ \
//     /_/               \_\
// (2)*---------------------* (1)
//              [0]  
// 
// face 0 : verts 0,1,2
// face 1 : verts 0,1,3
// face 2 : verts 0,2,3
// face 3 : verts 1,2,3
//
class SolverTetraeder : public SolverShape {
 public:
	 using VerticeIndexAndCoord 
		 = std::pair<VerticeIndexes::VerticeIndex, Point3D>;
	 using Indexes = std::array<VerticeIndexes::VerticeIndex, 4>;
	 SolverTetraeder(double thermal_conductivity,
		 double ambient_temperature,
		 double intensity_of_heat_source,
		 Indexes inp_indexes) : SolverShape(),
		 thermal_conductivity_(thermal_conductivity),
		 ambient_temperature_(ambient_temperature),
		 intensity_of_heat_source_(intensity_of_heat_source),
		 indexes_(inp_indexes) {}
	~SolverTetraeder() = default;
	const Indexes& indexes() const { return indexes_; }
	double thermal_conductivity() const { return thermal_conductivity_; }
	double ambient_temperature() const { return ambient_temperature_; }
	double intensity_of_heat_source() const { return intensity_of_heat_source_; }
 private:
	 double thermal_conductivity_;
	 double ambient_temperature_;
	 double intensity_of_heat_source_;
	 Indexes indexes_;
};

