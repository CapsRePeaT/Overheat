#include "temperature_bar_material.h"

#include <memory>

#include "config.h"
#include "renderer/shaderprogram.h"


namespace renderer {

TemperatureBarMaterial::TemperatureBarMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"temp_bar.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"temp_bar.fs")) {}

}  // namespace renderer