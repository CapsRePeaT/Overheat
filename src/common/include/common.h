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
  using Values = std::array<std::pair<float,float>, dim>;
  bool undefined() const { return coordinates_ == DefaultCoordinates(); }
  Box() : coordinates_(DefaultCoordinates()) {}
// TODO: make handy realisation with coords
  Box(Values values)
    : coordinates_(std::move(values)) {}
	Box(typename Values::value_type (&&values)[dim])
			: coordinates_(std::to_array(values)) {}
	[[nodiscard]] const Values& values()
      const {
    return coordinates_;
  }

 private:
  static constexpr Values DefaultCoordinates();
  Values coordinates_;
};

template<int dim>
constexpr typename Box<dim>::Values Box<dim>::DefaultCoordinates() {
  Values default_values;
  default_values.fill(std::make_pair(std::numeric_limits<float>::max(),std::numeric_limits<float>::min()));
  return default_values;
};

using Box3D = Box<3>;
using Box2D = Box<2>;

#endif // COMMON_H
