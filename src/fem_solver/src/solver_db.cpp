#include "solver_db.hpp"

bool FsDatapack::NextElement(SolverShape*& element) {
	if (elements_.empty())
		return false;
	element = elements_.back();
	elements_.pop_back();
	return true;
}

void FsDatapack::AddElement(SolverShape* element) {
	elements_.push_back(element);
}

void SolverHeatmap::Print() const {
	std::cout << "Heatmap temps:" << std::endl;
	for (size_t i = 0; i < temperatures_.size(); ++i)
		std::cout << i << ":\t" << temperatures_[i] << std::endl;
}
