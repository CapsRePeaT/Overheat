#pragma once

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"

class IDataProvider {
 public:
	virtual GeomStorage<BasicShape> geometry() = 0;
	virtual DefaultMetadataStorage metadata()  = 0;
	virtual HeatmapStorage heatmaps() = 0;

	~IDataProvider() = default;
};
