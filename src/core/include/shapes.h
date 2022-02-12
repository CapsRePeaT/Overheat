#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>
#include <memory>
#include <vector>

#include "../../common/include/common.h"

class BasicShape {
 public:
	using ShapePtr = std::shared_ptr<BasicShape>;
  using Shapes = std::vector<ShapePtr>;
	explicit BasicShape(const size_t id) : id_(id) {}
	BasicShape(const size_t id, Box3D bbox /* non const for move*/, 
						 const size_t layer_id)
			: id_(id), layer_id_(layer_id), bbox_(std::move(bbox)) {}
	size_t id() const { return id_; }
	const Box3D& bbox() const { return bbox_; }
	void setBox(Box3D box) { bbox_ = std::move(box); }

 private:
	const size_t id_ = UndefinedSizeT;
	const size_t layer_id_ = UndefinedSizeT;
	ShapeType shape_type_ = ShapeType::Undefined;
	Box3D bbox_;
};

#endif  // SHAPES_H
