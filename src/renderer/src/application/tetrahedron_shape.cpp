#include "tetrahedron_shape.h"

#include "renderer/renderer_api.h"

renderer::TetrahedronShape::TetrahedronShape(
		std::shared_ptr<SolverTetraeder> core_shape,
		std::shared_ptr<VertexBuffer> points, std::shared_ptr<VertexBuffer> temps,
		TetrahedronMaterial& material)
		: core_shape_(std::move(core_shape)), material_(&material) {
	auto& factory                       = RendererAPI::factory();
	const auto& point_indices           = core_shape_->indexes();
	std::array<uint32_t, 4 * 3> indices = {
			static_cast<uint32_t>(point_indices[0]),
			static_cast<uint32_t>(point_indices[1]),
			static_cast<uint32_t>(point_indices[2]),
			//
			static_cast<uint32_t>(point_indices[0]),
			static_cast<uint32_t>(point_indices[1]),
			static_cast<uint32_t>(point_indices[3]),
			//
			static_cast<uint32_t>(point_indices[0]),
			static_cast<uint32_t>(point_indices[2]),
			static_cast<uint32_t>(point_indices[3]),
			//
			static_cast<uint32_t>(point_indices[1]),
			static_cast<uint32_t>(point_indices[2]),
			static_cast<uint32_t>(point_indices[3])};
	auto&& ibo = factory.NewIndexBuffer(std::move(indices));
	std::vector<std::shared_ptr<VertexBuffer>> vbos = {std::move(points),
	                                                   std::move(temps)};
	vao_ = factory.NewVertexArray(std::move(vbos), std::move(ibo));
}

bool renderer::TetrahedronShape::SetContextForDraw(RendererContext& ctx) {
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
