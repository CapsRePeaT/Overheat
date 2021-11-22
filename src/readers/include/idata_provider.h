#pragma once

#include "databases.h"
#include "shapes.h"

class IDataProvider {
 public:
	virtual GeomStorage<BasicShape> geometry() = 0;
	virtual HeatmapStorage heatmap() = 0;
	virtual MetadataStorage metadata() = 0;

	~IDataProvider() = default;
};
