#ifndef DATABASES_H
#define DATABASES_H

#include <memory>
#include <vector>

template <class Shape>
class GeomStorage {
 public:
	using ShapePtr = std::shared_ptr<Shape>;
	using Shapes = std::vector<ShapePtr>;
	GeomStorage() = default;
	void AddShape(ShapePtr shape) { shapes_.emplace_back(std::move(shape)); }
	const Shapes& get_all_shapes() const { return shapes_; }
	void Clear() { shapes_.clear(); }

 private:
	Shapes shapes_;
};

// not sure that we need it now as we have only one heatmap
//Shitty class with net and temperatures
//TODO: Fix it in future to make handy
class HeatmapStorage {
 public:
	HeatmapStorage() = default;

  size_t layers_count;
  std::vector<float> x;
  std::vector<float> y;
  std::vector<float> temperature;
};

class MetadataStorage {
 public:
	MetadataStorage() = default;
};

#endif  // DATABASES_H
