#ifndef HEATMAP_H
#define HEATMAP_H

#include <limits>
#include <vector>

class Heatmap {
 public:
	explicit Heatmap(std::vector<float> temperature_mv);
	// TBD: <some_type> operator[](std::size_t idx); ?
	[[nodiscard]] float min_temp() const { return min_temp_; }
	[[nodiscard]] float max_temp() const { return max_temp_; }

 private:
	// for alexey purposes we want to use single mem array, so we implement 2d
	// array access on out own
	std::vector<float> temperature_;
	float max_temp_ = std::numeric_limits<float>::min();
	float min_temp_ = std::numeric_limits<float>::max();
};

#endif  // HEATMAP_H
