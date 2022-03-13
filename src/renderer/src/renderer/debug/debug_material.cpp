#include "debug_material.h"

namespace renderer {
namespace debug {

DebugMaterial::DebugMaterial()
		: shader_(std::make_shared<ShaderProgram>(
					// TODO: make resource manager to load shaders
					std::filesystem::current_path() / "res" / "shaders" / "debug.vs",
					std::filesystem::current_path() / "res" / "shaders" / "debug.fs")) {}
}  // namespace debug
}  // namespace renderer
