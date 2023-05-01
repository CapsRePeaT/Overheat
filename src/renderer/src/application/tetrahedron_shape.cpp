#include "tetrahedron_shape.h"

#include "renderer/renderer_api.h"

namespace renderer {

TetrahedronShape::TetrahedronShape(
		const std::vector<std::shared_ptr<SolverTetraeder>>& core_shapes,
		std::shared_ptr<VertexBuffer> points, std::shared_ptr<VertexBuffer> temps,
		TetrahedronMaterial& material, const std::vector<glm::vec3>& coords)
		: material_(&material) {
	constexpr float max_f = std::numeric_limits<float>::max();
	constexpr float min_f = -max_f;

	glm::vec3 bb_min = {max_f, max_f, max_f};
	glm::vec3 bb_max = {min_f, min_f, min_f};
	auto& factory    = RendererAPI::factory();

	std::vector<glm::u32vec3> indices;
	indices.reserve(core_shapes.size() * 4);
	// can be optimized by not drawing internal triangles
	for (auto& core_shape : core_shapes) {
		const auto& point_indices = core_shape->indexes();
		for (auto idx : point_indices) {
			auto coord = coords[idx];
			for (size_t i = 0; i < 3; ++i) {
				bb_min[i] = std::min(bb_min[i], coord[i]);
				bb_max[i] = std::max(bb_max[i], coord[i]);
			}
		}
		indices.push_back({static_cast<uint32_t>(point_indices[0]),
		                   static_cast<uint32_t>(point_indices[1]),
		                   static_cast<uint32_t>(point_indices[2])});
		indices.push_back({static_cast<uint32_t>(point_indices[0]),
		                   static_cast<uint32_t>(point_indices[1]),
		                   static_cast<uint32_t>(point_indices[3])});
		indices.push_back({static_cast<uint32_t>(point_indices[0]),
		                   static_cast<uint32_t>(point_indices[2]),
		                   static_cast<uint32_t>(point_indices[3])});
		indices.push_back({static_cast<uint32_t>(point_indices[1]),
		                   static_cast<uint32_t>(point_indices[2]),
		                   static_cast<uint32_t>(point_indices[3])});
	}
	auto&& ibo = factory.NewIndexBuffer(&indices[0][0], indices.size() * 3);
	std::vector<std::shared_ptr<VertexBuffer>> vbos = {std::move(points),
	                                                   std::move(temps)};
	vao_ = factory.NewVertexArray(std::move(vbos), std::move(ibo));

	BasicShape shape({}, {},
	                 Box3D({std::make_pair(bb_min.x, bb_max.x),
	                        std::make_pair(bb_min.y, bb_max.y),
	                        std::make_pair(bb_min.z, bb_max.z)}));
	shape_for_wireframe_ = std::make_unique<BoxShape>(std::move(shape));
}

bool TetrahedronShape::SetContextForDraw(RendererContext& ctx) {
	if (!is_visible())
		return false;
	ctx.SetVao(vertex_array());
	ctx.SetMaterialCallbacks(
			[this, &material = *material_, &ctx]() {
				material.Use(transform(), ctx.camera().viewProjectionMatrix(),
		                 !is_visible());
			},
			[&material = *material_]() { material.Unuse(); });
	return true;
}

bool TetrahedronShape::SetContextForDrawWireframe(RendererContext& ctx) {
	if (!is_visible() && highlight_type() == HighlightType::None)
		return false;
	ctx.SetVao(shape_for_wireframe_->wireframe_vao());
	ctx.SetMaterialCallbacks(
			[this, &ctx]() {
				ctx.wireframe_material().Use(
						transform(), ctx.camera().viewProjectionMatrix(),
						HighlightTypeToColor(highlight_type()));
			},
			[&ctx]() { ctx.wireframe_material().Unuse(); });
	return true;
}

}  // namespace renderer
