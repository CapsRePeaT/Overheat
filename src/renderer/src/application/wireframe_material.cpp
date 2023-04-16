#include "wireframe_material.h"

#include "config.h"

renderer::WireframeMaterial::WireframeMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"wireframe.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"wireframe.fs")) {}
