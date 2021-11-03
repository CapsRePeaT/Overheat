#include "core.h"


Core::Shapes Core::GetShapes(const Box3D& area) {
  return geom_storage_.get_all_shapes();
}

void Core::LoadGeometry(const std::string& file_name) {
  // TODO make proper interface class usage
  DummyReader reader;
	geom_storage_.Clear();
  reader.LoadGeometry(file_name, geom_storage_);
}
