#pragma once

#include "solver_db.hpp"
#include "../../core/include/databases.h"

class GeometryCutter {
 public:
  GeometryCutter() = default;
  ~GeometryCutter() = default;
  FsDatapack PrepareGeometry(FileRepresentation& file_rep);
  const VerticeIndexes& GetVerticeIndexes();
 private:
  VerticeIndexes index_to_coord_map_;
};
