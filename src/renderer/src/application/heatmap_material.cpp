#include "heatmap_material.h"

#include "config.h"

namespace renderer {

HeatmapMaterial::HeatmapMaterial(
		std::pair<std::unique_ptr<Texture2D>, std::unique_ptr<Texture2D>>
				heatmap_textures,
		std::pair<glm::vec2, glm::vec2> bot_top_temp_ranges,
		std::pair<float, float> bounds)
		: shader_(std::make_shared<ShaderProgram>(
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"heatmap.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"heatmap.fs")),
			bot_temp_range_(bot_top_temp_ranges.first),
			top_temp_range_(bot_top_temp_ranges.second),
			bot_heatmap_texture_(std::move(heatmap_textures.first)),
			top_heatmap_texture_(std::move(heatmap_textures.second)),
			bounds_({{0.f, 0.f}, {bounds.first, bounds.second}}) {}

}  // namespace renderer
