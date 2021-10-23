#ifndef SHAPES_H
#define SHAPES_H

#include <stdio.h>
#include "common.h"

class BasicShape
{
 public:
  BasicShape();
  size_t id() const { return id_; }
 private:
  size_t id_;
  ShapeType shape_type_ = ShapeType::Undefined;
  Box3D bbox_;
};

#endif // SHAPES_H
