#include "databases.h"

const FileRepresentation::Shapes& FileRepresentation::GetShapes(const Box3D& area) const {
	return geom_storage_.get_all_shapes();
}
