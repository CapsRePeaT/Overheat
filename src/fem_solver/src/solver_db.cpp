#include "solver_db.hpp"

//bool FsDatapack::NextElement(SolverShape*& element) {
//	if (elements_.empty())
//		return false;
//	element = elements_.back();
//	elements_.pop_back();
//	return true;
//}

void FsDatapack::AddElement(SolverShape* element) {
	// TODO switch to unique_ptr
	elements_.emplace_back(std::shared_ptr<SolverShape>(element));
}

void SolverHeatmap::FillData(const SparceMatrix& matrix) {
	assert(temperatures_.empty() && "Heatmap not adapted for rewriting");
	double sum = 0;
	for (unsigned i = 0; i < matrix.size1(); ++i)
		temperatures_.push_back(matrix(i, 0));
	ComputeMinMax();
}

void SolverHeatmap::ComputeMinMax() {
	double sum = 0;
	for (const auto temp : temperatures_) {
		if (temp < min_temp_)
			min_temp_ = temp;
		if (temp > max_temp_)
			max_temp_ = temp;
		sum += temp;
	}
	avg_temp_ = sum / temperatures_.size();
}

void SolverHeatmap::Print(bool detailed_print) const {
	std::cout << "Heatmap temps:" << std::endl;
	std::cout << "-> contains " << temperatures_.size() 
		      << " points" << std::endl;
	std::cout << "-> min temp: " << min_temp() << std::endl;
	std::cout << "-> max temp: " << max_temp() << std::endl;
	std::cout << "-> avg temp: " << avg_temp() << std::endl;
	if (detailed_print) {
		std::cout << "-> first 30 temps:" << std::endl;
		for (size_t i = 0; i < 30; ++i) {
			if (i >= temperatures_.size())
				break;
			std::cout << i << ":\t" << temperatures_[i] << std::endl;
		}
	}
}

