#pragma once

#include <array>
#include <cassert>
#include <limits>
#include <set>
#include <string>
#include <vector>

constexpr size_t UndefinedSizeT       = std::numeric_limits<size_t>::max();
constexpr size_t UndefinedId          = UndefinedSizeT;
constexpr size_t DefaultMatrixSize    = 0;
constexpr double DefaultRoundBoundary = 100000.0;

using ShapeId          = size_t;
using LayerId          = size_t;
using HeatmapId        = size_t;
using RepresentationId = size_t;

using Floats = std::vector<float>;

enum class DrawMode { Gradient = 0, Stratified };

// weird option, may be not needed in the future
enum class ShowTexture { Above = 0, Below, Both };

enum class HighlightType { None, Selected, ActiveSelected };

enum class ShapeType { Box = 0, Sphere, Count, Undefined = Count };

enum class InstanceType {
	Shape = 0,
	Layer,
	Representation,
	Count,
	Undefined = Count
};

class GlobalId {
 public:
	using InstanceId = size_t;
	GlobalId()       = default;
	constexpr GlobalId(const InstanceType type, const InstanceId id,
	                   const RepresentationId representation_id)
			: type_(type), id_(id), representation_id_(representation_id) {}
	GlobalId(const GlobalId& other)                             = default;
	[[nodiscard]] bool operator==(const GlobalId& other) const  = default;
	[[nodiscard]] auto operator<=>(const GlobalId& other) const = default;  //{
	// 	if (representation_id_ != other.representation_id_)
	// 		return representation_id_ <=> other.representation_id_;
	// 	if (type_ != other.type_)
	// 		return static_cast<int>(type_) <=> static_cast<int>(other.type_);
	// 	return id_ <=> other.id_;
	// }
	[[nodiscard]] constexpr InstanceId id() const { return id_; }
	[[nodiscard]] constexpr RepresentationId representation_id() const {
		return representation_id_;
	}
	[[nodiscard]] constexpr InstanceType type() const { return type_; }

 private:
	InstanceType type_                  = InstanceType::Undefined;
	InstanceId id_                      = UndefinedId;
	RepresentationId representation_id_ = UndefinedId;
};

struct InstanceList {
	InstanceList(const GlobalId init_id) : id(init_id) {}
	std::string name;
	GlobalId id;
	std::vector<InstanceList> dependants;
};

using GlobalIds = std::vector<GlobalId>;

enum class GeometryType { D3 = 0, D2, Undefined };

enum Axis { X = 0, Y, Z, Count, Undefined = Count };

struct Point3D {
	// TODO: consider using double, cause cinolib uses doubles
	std::array<double, 3> coords;
	friend bool operator<(const Point3D& l, const Point3D& r) {
		return std::tie(l.coords[0], l.coords[1], l.coords[2]) <
		       std::tie(r.coords[0], r.coords[1], r.coords[2]);
	}
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
	Box(typename Values::value_type (&&values)[dim])
			: coordinates_(std::to_array(values)) {}
	[[nodiscard]] const Values& coordinates() const { return coordinates_; }
	[[nodiscard]] bool undefined() const {
		return coordinates_ == DefaultCoordinates();
	}
	float width(const int needed_dim) const {
		assert(needed_dim >= dim);
		return coordinates_[needed_dim].second() - coordinates_[needed_dim].first();
	}

 private:
	static constexpr Values DefaultCoordinates();
	Values coordinates_;
};

using Box3D = Box<3>;
using Box2D = Box<2>;

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

// Why is not unordered_set?
using MetadataPack = std::vector<PreparedMetadata>;
