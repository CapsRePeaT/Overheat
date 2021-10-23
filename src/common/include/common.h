#ifndef COMMON_H
#define COMMON_H
#include <array>
#include <limits>

enum class DrawMode {Gradient, Stratified};
enum class ShapeType {Undefined};
enum Axis {X,Y,Z};

// represents 3d box //should we use dimention as template parameter?
// array of min and max cords by dimention, by default values from numeric limits
template<int dim>
class Box {
 public:
  bool undefined() const { return coordinates_ == DefaultCoordinates(); }
  Box() : coordinates_(DefaultCoordinates()) {}
 private:
  using BoxArray = std::array<std::pair<float,float>, dim>;
  static constexpr BoxArray DefaultCoordinates();
  BoxArray coordinates_;
};

template<int dim>
constexpr typename Box<dim>::BoxArray Box<dim>::DefaultCoordinates() {
  BoxArray boxes;
  boxes.fill(std::make_pair(std::numeric_limits<float>::max(),std::numeric_limits<float>::min()));
  return boxes;
};

using Box3D = Box<3>;
using Box2D = Box<2>;

#endif // COMMON_H
