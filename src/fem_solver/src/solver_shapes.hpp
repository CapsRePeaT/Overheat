#pragma once

class SolverShape {
 public:
	SolverShape() = default;
	virtual ~SolverShape() = default;
};

class SolverTetraeder : public SolverShape {
 public:
	SolverTetraeder();
	~SolverTetraeder();

 private:
};

