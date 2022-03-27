#include "type2_data_provider.h"

namespace Readers::Type2 {
Type2DataProvider::Type2DataProvider(const type2_TRM& geom) {
	load_geometry(geom);
}

void Type2DataProvider::load_geometry(const type2_TRM& data) { geometry_.Clear(); }

}  // namespace Readers
