#pragma once

#include <array>
#include <limits>

enum class DrawMode { Gradient, Stratified };
enum class ShapeType { Undefined };
enum Axis { X, Y, Z };

// represents 3d box //should we use dimension as template parameter?
// array of min and max cords by dimension, by default values from numeric
// limits
template <int dim>
class Box {
 public:
	using Values = std::array<std::pair<float, float>, dim>;
	// TODO: this implementation doesn't give any benefits (and may be even
	// slower!), compared to direct `coordinates_.fill(...)` in ctor body
	Box() : coordinates_(DefaultCoordinates()) {}
	// TODO: make handy realization with coords
	Box(Values values) : coordinates_(std::move(values)) {}
	Box(typename Values::value_type(&&values)[dim])
			: coordinates_(std::to_array(values)) {}
	[[nodiscard]] const Values& coordinates() const { return coordinates_; }
	[[nodiscard]] bool undefined() const {
		return coordinates_ == DefaultCoordinates();
	}

 private:
	static constexpr Values DefaultCoordinates();
	Values coordinates_;
};

template <int dim>
constexpr typename Box<dim>::Values Box<dim>::DefaultCoordinates() {
	Values default_values;
	default_values.fill(std::make_pair(std::numeric_limits<float>::max(),
	                                   std::numeric_limits<float>::min()));
	return default_values;
};

using Box3D = Box<3>;
using Box2D = Box<2>;
