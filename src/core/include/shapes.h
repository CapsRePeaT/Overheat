#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>
#include <memory>
#include <vector>

#include "../../common/include/common.h"

class ShapeId : public std::pair<size_t, size_t> {
 public:
	using Id  = size_t;
	ShapeId(const RepresentationId design_id, const Id id)
			: std::pair<size_t, size_t>(id, design_id) {}
	Id id() const { return first; }
	RepresentationId design_id() const { return second; }
};

using ShapeIds = std::vector<ShapeId>;

class BasicShape {
 public:
	explicit BasicShape(const ShapeId id) : id_(id) {}
	BasicShape(const ShapeId id,
	           const size_t layer_id,
						 Box3D bbox /* non const for move*/)
			: id_(id), layer_id_(layer_id), bbox_(std::move(bbox)) {}
	ShapeId id() const { return id_; }
	const Box3D& bbox() const { return bbox_; }
	void setBox(Box3D box) { bbox_ = std::move(box); }

 private:
	const ShapeId id_;
	const size_t layer_id_ = UndefinedSizeT;
	ShapeType shape_type_ = ShapeType::Undefined;
	Box3D bbox_;
};

using ShapePtr = std::shared_ptr<BasicShape>;
using Shapes = std::vector<ShapePtr>;

#endif  // SHAPES_H
