#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>

#include "../../common/include/common.h"

class BasicShape {
 public:
	explicit BasicShape(const size_t id) : id_(id) {}
	BasicShape(const size_t id, const Box3D& bbox,
	           ShapeType shape_type = ShapeType::Undefined)
			: id_(id), bbox_(bbox), shape_type_(shape_type) {}
	size_t id() const { return id_; }
	const Box3D& bbox() const { return bbox_; }
	void setBox(Box3D box) { bbox_ = box; };

 private:
	size_t id_;
	ShapeType shape_type_ = ShapeType::Undefined;
	Box3D bbox_;
};

#endif  // SHAPES_H
