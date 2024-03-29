#include "box_shape.h"

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_quat.hpp>

#include <memory>

#include "renderer/renderer_api.h"
#include "renderer/vertexbuffer.h"
#include "renderer/vertexbufferlayout.h"

namespace renderer {

// TODO: handle vertices better
struct Vertex {
	glm::vec3 positions;
	glm::vec2 uv_coordinates;
	glm::vec2 side_uv_coordinates;
};

struct WireframeVertex {
	glm::vec3 positions;
};

// VertexBufferLayout doesn't support non-contigious vertices now, so we
// should check it
static_assert(sizeof(Vertex) ==
                  sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec2) &&
              "All data in vertex struct must be contigious");
static_assert(sizeof(glm::vec3) == sizeof(float) * 3 &&
              "All data in vertex struct must be contigious");
static_assert(sizeof(glm::vec2) == sizeof(float) * 2 &&
              "All data in vertex struct must be contigious");

BoxShape::BoxShape(const BasicShape& shape) : core_shape_(shape) {
	// (minX, maxX), (minY, maxY), (minZ, maxZ)
	const auto bounds = shape.bbox().coordinates();

	// Just to name values
	const glm::vec3 min_bounds = {bounds[0].first, bounds[1].first,
	                              bounds[2].first};
	const glm::vec3 max_bounds = {bounds[0].second, bounds[1].second,
	                              bounds[2].second};

	//       5---------6
	//      /|        /|
	//     / |       / |
	//    /  |      /  |
	//   /   1-----/---2
	//  /   /     /   /
	// 4---------7   /    ^ z
	// |  /      |  /     |  ^ y
	// | /       | /      | /
	// |/        |/       |/
	// 0---------3        o------> x
	//
	const std::array<const Vertex, 8> vertices = {
			Vertex{{min_bounds.x, min_bounds.y, min_bounds.z},
	           {0.0f, 0.0f},
	           {0.0f, 0.0f}},  // 0
			Vertex{{min_bounds.x, max_bounds.y, min_bounds.z},
	           {0.0f, 1.0f},
	           {0.0f, 1.0f}},  // 1
			Vertex{{max_bounds.x, max_bounds.y, min_bounds.z},
	           {1.0f, 1.0f},
	           {0.0f, 0.0f}},  // 2
			Vertex{{max_bounds.x, min_bounds.y, min_bounds.z},
	           {1.0f, 0.0f},
	           {0.0f, 1.0f}},  // 3
			Vertex{{min_bounds.x, min_bounds.y, max_bounds.z},
	           {0.0f, 0.0f},
	           {1.0f, 0.0f}},  // 4
			Vertex{{min_bounds.x, max_bounds.y, max_bounds.z},
	           {0.0f, 1.0f},
	           {1.0f, 1.0f}},  // 5
			Vertex{{max_bounds.x, max_bounds.y, max_bounds.z},
	           {1.0f, 1.0f},
	           {1.0f, 0.0f}},  // 6
			Vertex{{max_bounds.x, min_bounds.y, max_bounds.z},
	           {1.0f, 0.0f},
	           {1.0f, 1.0f}},  // 7
	};

	// 3 indices for a triangle, 2 triangles for a face, 6 faces
	// Counter-clockwise direction to see front face
	constexpr std::array<uint32_t, 3 * 2 * 6> raw_ibo = {
			0, 1, 3, 3, 1, 2,  // face 0-1-2-3
			0, 3, 4, 4, 3, 7,  // face 0-4-7-3
			0, 4, 1, 1, 4, 5,  // face 0-4-5-1
			6, 7, 2, 2, 7, 3,  // face 6-7-3-2
			6, 2, 5, 5, 2, 1,  // face 6-5-1-2
			6, 5, 7, 7, 5, 4   // face 6-7-4-5
	};
	auto&& layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(3);
	layout->Push<float>(2);
	layout->Push<float>(2);
	auto& factory = RendererAPI::factory();
	std::vector<std::shared_ptr<VertexBuffer>> vbos;
	auto&& vbo = factory.NewVertexBuffer(vertices, std::move(layout));
	vbos.emplace_back(std::move(vbo));
	auto&& ibo = factory.NewIndexBuffer(raw_ibo);
	vao_       = factory.NewVertexArray(std::move(vbos), std::move(ibo));

	std::array<WireframeVertex, 8> wf_vertices;
	for (size_t i = 0; i < wf_vertices.size(); ++i) {
		wf_vertices[i].positions = vertices[i].positions;
	};
	constexpr std::array<uint32_t, 8 + 1 + 3 + 1 + 3 + 1 + 2> wf_raw_ibo = {
			0, 4, 7, 3, 0, 1, 5, 4, 
			std::numeric_limits<uint32_t>::max(),
			5, 6, 7,
			std::numeric_limits<uint32_t>::max(),
			1, 2, 3,
			std::numeric_limits<uint32_t>::max(),
			6, 2
	};
	auto&& wf_layout = std::make_unique<VertexBufferLayout>();
	wf_layout->Push<float>(3);
	std::vector<std::shared_ptr<VertexBuffer>> wf_vbos;
	auto&& wf_vbo = factory.NewVertexBuffer(wf_vertices, std::move(wf_layout));
	wf_vbos.emplace_back(std::move(wf_vbo));
	auto&& wf_ibo = factory.NewIndexBuffer(wf_raw_ibo);
	wireframe_vao_ =
			factory.NewVertexArray(std::move(wf_vbos), std::move(wf_ibo));
}

bool BoxShape::SetContextForDraw(RendererContext& ctx) {
	if (!is_visible() && highlight_type() == HighlightType::None)
		return false;
	ctx.SetVao(vertex_array());
	ctx.SetMaterialCallbacks(
			[this, &material = *material_, &ctx]() {
				material.Use(transform(), ctx.camera().viewProjectionMatrix(),
		                 HighlightTypeToColor(highlight_type()), !is_visible());
			},
			[&material = *material_]() { material.Unuse(); });
	return true;
}

bool BoxShape::SetContextForDrawWireframe(RendererContext& ctx) {
	if (!is_visible() && highlight_type() == HighlightType::None)
		return false;
	ctx.SetVao(*wireframe_vao_);
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
