#pragma once

#include <memory>
#include <vector>

#include "../../common/include/common.h"

using Id          = size_t;
using ShapeIdPair = std::pair<Id, RepresentationId>;

class ShapeId : public ShapeIdPair {
 public:
	constexpr ShapeId(const Id id, const RepresentationId design_id)
			: ShapeIdPair(id, design_id) {}
	constexpr ShapeId(const ShapeIdPair& ids)  // NOLINT(google-explicit-constructor)
			: ShapeIdPair(ids) {}
	[[nodiscard]] constexpr Id id() const { return first; }
	[[nodiscard]] constexpr RepresentationId design_id() const { return second; }
};

using ShapeIds = std::vector<ShapeId>;

class BasicShape {
 public:
	explicit BasicShape(const ShapeId id) : id_(id) {}
	BasicShape(const ShapeId id, const size_t layer_id, Box3D bbox_mv)
			: id_(id), layer_id_(layer_id), bbox_mv_(std::move(bbox_mv)) {}
	[[nodiscard]] ShapeId id() const { return id_; }
	[[nodiscard]] const Box3D& bbox() const { return bbox_mv_; }
	void setBox(Box3D box) { bbox_mv_ = std::move(box); }

 private:
	const ShapeId id_;
	const size_t layer_id_ = UndefinedSizeT;
	ShapeType shape_type_  = ShapeType::Undefined;
	Box3D bbox_mv_;
};

using ShapePtr = std::shared_ptr<BasicShape>;
using Shapes   = std::vector<ShapePtr>;
