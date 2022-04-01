#ifndef HEATMAP_H
#define HEATMAP_H

#include <limits>
#include <vector>

// maybe better to inmpement it in multy dimentional format (3D, 2D, 1D) and
// make dimention as template ardument ?
// template <int N>
class Heatmap {
 public:
	Heatmap(std::vector<float> temperature_mv);
	// some tipe? operator[](std::size_t idx);
	[[nodiscard]] float min_temp() const { return min_temp_; }
	[[nodiscard]] float max_temp() const { return max_temp_; }

 private:
	// for alexey purposes we whant to use single mem array, so we implement 2d
	// array acess on out own
	std::vector<float> temperature_;
	[[nodiscard("")]] float max_temp_ = std::numeric_limits<float>::min();
	[[nodiscard("")]] float min_temp_ = std::numeric_limits<float>::max();
};

#endif  // HEATMAP_H
