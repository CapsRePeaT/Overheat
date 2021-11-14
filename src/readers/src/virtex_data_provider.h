#pragma once

class VirtexDataProvider : public IDataProvider {
 public:
	explicit VirtexDataProvider(const VirtexData& data) {}

	GeomStorage<BasicShape*> geometry() override { return {}; }
	HeatmapStorage heatmap() override { return {}; }
	MetadataStorage metadata() override { return {}; };
};