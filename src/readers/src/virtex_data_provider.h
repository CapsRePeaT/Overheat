#pragma once

class VirtexDataProvider : public IDataProvider {
 public:
	explicit VirtexDataProvider(const VirtexData& data) {}

	GeomStorage<BasicShape*> getGeometry() override { return {}; }
	// Сan we use std::ostream instead?
	HeatmapStorage getHeatmap() override { return {}; }
	MetadataStorage getMetadata() override { return {}; };
};