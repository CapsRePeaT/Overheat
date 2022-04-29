#include "heatmap_material.h"

#include <filesystem>

#include "config.h"

namespace renderer {

HeatmapMaterial::HeatmapMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					// TODO: make resource manager to load shaders
					Config::instance().executable_dir_path() / "res" / "shaders" / "heatmap.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" / "heatmap.fs")) {
}

}  // namespace renderer
