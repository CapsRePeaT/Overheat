#include "heatmap_material.h"

#include <filesystem>

HeatmapMaterial::HeatmapMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					// TODO: make resource manager to load shaders
					std::filesystem::current_path() / "res" / "shaders" / "heatmap.vs",
					std::filesystem::current_path() / "res" / "shaders" / "heatmap.fs")) {}
