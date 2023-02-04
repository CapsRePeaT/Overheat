#pragma once

#include <iostream>

#include "databases.h"
#include "idata_provider.h"
#include "shapes.h"


class IReader {
 public:
	// TODO: we should return new class FileRepresentation
	GeomStorage<BasicShape> geometry() { return data_provider_->geometry(); }
	DefaultMetadataStorage metadata() { return data_provider_->metadata(); }
	HeatmapStorage heatmaps() { return data_provider_->heatmaps(); }
	LayersShapes layers_shapes() { return data_provider_->layers_shapes(); }


 protected:
	virtual void load() = 0;
	std::unique_ptr<IDataProvider> data_provider_;
	virtual ~IReader() = default;
};
