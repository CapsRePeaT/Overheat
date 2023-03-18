#include "font_material.h"

#include "config.h"
#include "renderer/shaderprogram.h"

renderer::FontMaterial::FontMaterial(std::shared_ptr<Texture2D> font_atlas)
		: atlas_(font_atlas), shader_(std::make_shared<ShaderProgram>(
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"font.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"font.fs")) {}
