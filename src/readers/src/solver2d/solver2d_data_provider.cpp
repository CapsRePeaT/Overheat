#include "solver2d_data_provider.h"

namespace Readers::Solver2d {
Solver2dDataProvider::Solver2dDataProvider(const Solver2d_TRM& geom) {
	load_geometry(geom);
}

void Solver2dDataProvider::load_geometry(const Solver2d_TRM& data) { geometry_.Clear(); }

}  // namespace Readers
