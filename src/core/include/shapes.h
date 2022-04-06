#pragma once

#include <memory>
#include <vector>

#include "../../common/include/common.h"

class GlobalShapeId {
 public:
	constexpr GlobalShapeId(const ShapeId id, const RepresentationId design_id)
			: id_(id), representation_id_(design_id) {}
	[[nodiscard]] bool operator==(const GlobalShapeId& other) const = default;
	[[nodiscard]] constexpr ShapeId id() const { return id_; }
	[[nodiscard]] constexpr RepresentationId representation_id() const {
		return representation_id_;
	}

 private:
	ShapeId id_                         = UndefinedId;
	RepresentationId representation_id_ = UndefinedId;
};

using GlobalShapeIds = std::vector<GlobalShapeId>;

class BasicShape {
 public:
	explicit BasicShape(const GlobalShapeId id) : id_(id) {}
	BasicShape(const GlobalShapeId id, const LayerId layer_id, Box3D bbox_mv)
			: id_(id), layer_id_(layer_id), bbox_(std::move(bbox_mv)) {}
	[[nodiscard]] GlobalShapeId id() const { return id_; }
	[[nodiscard]] const Box3D& bbox() const { return bbox_; }
	void setBox(Box3D box_mv) { bbox_ = std::move(box_mv); }

 private:
	const GlobalShapeId id_;
	const LayerId layer_id_ = UndefinedId;
	ShapeType shape_type_   = ShapeType::Undefined;
	Box3D bbox_;
};

using ShapePtr = std::shared_ptr<BasicShape>;
using Shapes   = std::vector<ShapePtr>;
