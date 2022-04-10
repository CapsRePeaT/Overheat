#pragma once

#include <array>
#include <limits>
#include <set>
#include <string>

enum class DrawMode { 
	Gradient = 0, 
	Stratified 
};

enum class ShapeType { 
	Box = 0, 
	Sphere, 
	count,
	Undefined = count
};

enum class GeometryType { 
	D3 = 0, 
	D2, 
	Undefined
};

enum Axis { 
	X = 0, 
	Y, 
	Z, 
	count, 
	Undefined = count 
};

// represents 3d box //should we use dimension as template parameter?
// array of min and max cords by dimension, by default values from numeric
// limits
template <int dim>
class Box {
 public:
	// TODO: move hide Values in private, make api to get coordinates values
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

class PreparedMetadata {
 public:
	PreparedMetadata(std::string meaning_mv, std::string value_mv)
			: meaning_(std::move(meaning_mv)), value_(std::move(value_mv)) {}
	[[nodiscard]] const std::string& meaning() const { return meaning_; }
	[[nodiscard]] const std::string& value() const { return value_; }

 private:
	std::string meaning_;
	std::string value_;
};

constexpr size_t UndefinedSizeT = std::numeric_limits<size_t>::max();
constexpr size_t UndefinedId    = UndefinedSizeT;

using Box3D            = Box<3>;
using Box2D            = Box<2>;
using ShapeId          = size_t;
using RepresentationId = size_t;
using LayerId          = size_t;
using HeatmapId        = size_t;
// Why is not unordered_set?
using MetadataPack = std::set<PreparedMetadata>;
