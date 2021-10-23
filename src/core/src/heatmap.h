#ifndef HEATMAP_H
#define HEATMAP_H

#include <vector>
#include <limits>


// maybe better to inmpement it in multy dimentional format (3D, 2D, 1D) and make
// dimention as template ardument ?
template<int N>
class Heatmap
{
 public:
  Heatmap();
  // some tipe? operator[](std::size_t idx);
  float min_temp() const { return min_temp_; }
  float max_temp() const { return max_temp_; }

 private:
  // for alexey purposes we whant to use single mem array, so we implement 2d array acess on out own
  std::vector<float> raw_data_;
  float max_temp_ = std::numeric_limits<float>::min();
  float min_temp_ = std::numeric_limits<float>::max();
};

#endif // HEATMAP_H
