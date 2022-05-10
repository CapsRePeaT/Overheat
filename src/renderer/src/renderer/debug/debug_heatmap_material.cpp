#include "debug_heatmap_material.h"

#include <filesystem>

#include "config.h"

namespace renderer::debug {

DebugHeatmapMaterial::DebugHeatmapMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					// TODO: make resource manager to load shaders
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"debug_heatmap.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"debug_heatmap.fs")) {}

}  // namespace renderer::debug
