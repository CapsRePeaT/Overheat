#ifndef COMMON_H
#define COMMON_H

enum class DrawMode {Gradient, Stratified};
enum class ShapeType {Undefined};

// represents 3d box //should we use dimention as template parameter?
// array of min and max cords by dimention, by default values from numeric limits
class BBox3D {
 public:
  bool undefined() const;
};

#endif // COMMON_H
