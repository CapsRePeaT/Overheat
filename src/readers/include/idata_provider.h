#pragma once

#include "../../common/include/shapes.h"
#include "../../core/src/databases.h"
class IDataProvider {
 public:
	virtual GeomStorage<BasicShape*> getGeometry() = 0;
	virtual HeatmapStorage getHeatmap() = 0;
	virtual MetadataStorage getMetadata() = 0;

	~IDataProvider() = default;
};