#ifndef DATABASES_H
#define DATABASES_H


template <class Shape>
class GeomStorage {
 public:
  GeomStorage() = default;
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

#endif // DATABASES_H
