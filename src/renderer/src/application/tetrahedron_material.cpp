#include "tetrahedron_material.h"

#include "config.h"

namespace renderer {

TetrahedronMaterial::TetrahedronMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"tetrahedrons.vs",
					Config::instance().executable_dir_path() / "res" / "shaders" /
							"tetrahedrons.fs")) {}

}  // namespace renderer
