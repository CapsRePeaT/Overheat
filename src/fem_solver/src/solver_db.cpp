#include "solver_db.hpp"

//bool FsDatapack::NextElement(SolverShape*& element) {
//	if (elements_.empty())
//		return false;
//	element = elements_.back();
//	elements_.pop_back();
//	return true;
//}

void FsDatapack::AddElement(SolverShape* element) {
	elements_.push_back(element);
}

void SolverHeatmap::Print() const {
	double min_temp = std::numeric_limits<double>::max();
	double max_temp = std::numeric_limits<double>::min();
	double sum = 0;
	for (size_t i = 0; i < temperatures_.size(); ++i) {
		if (temperatures_[i] < min_temp)
			min_temp = temperatures_[i];
		if (temperatures_[i] > max_temp)
			max_temp = temperatures_[i];
		sum += temperatures_[i];
	}
	std::cout << "Heatmap temps:" << std::endl;
	std::cout << "-> contains " << temperatures_.size() 
		      << " points" << std::endl;
	std::cout << "-> min temp: " << min_temp << std::endl;
	std::cout << "-> max temp: " << max_temp << std::endl;
	std::cout << "-> avg temp: " << sum / temperatures_.size() << std::endl;
	std::cout << "-> first 30 temps:" << std::endl;
	for (size_t i = 0; i < 30; ++i) {
		if (i >= temperatures_.size())
			break;
		std::cout << i << ":\t" << temperatures_[i] << std::endl;
	}
		
}
