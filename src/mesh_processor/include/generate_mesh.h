#define pragma once

#include <cinolib/meshes/drawable_tetmesh.h>

#include "../../core/include/databases.h"

namespace MeshProcessor{
using TetmeshVec = std::vector<cinolib::DrawableTetmesh<>>;

TetmeshVec generate(const LayersShapes& layers);
}
