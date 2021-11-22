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
	const Shapes& get_all_shapes() { return shapes_; }
	void Clear() { shapes_.clear(); }

 private:
	Shapes shapes_;
};

// not sure that we need it now as we have only one heatmap
class HeatmapStorage {
 public:
	HeatmapStorage() = default;
};

class MetadataStorage {
 public:
	MetadataStorage() = default;
};

#endif  // DATABASES_H
