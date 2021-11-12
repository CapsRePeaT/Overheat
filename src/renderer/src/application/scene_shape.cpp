#include "scene_shape.h"

#include <memory>

#include "renderer/vertexbuffer.h"
#include "renderer/vertexbufferlayout.h"

// TODO: handle vertices better
struct Vertex {
	glm::vec3 positions;
	glm::vec2 uv_coordinates;
};

SceneShape::SceneShape(const BasicShape& shape) : id_(shape.id()) {
	// (minX, maxX), (minY, maxY), (minZ, maxZ)
	auto bounds = shape.bbox().values();

	// Just to name values
	glm::vec3 min_bounds = {bounds[0].first, bounds[1].first, bounds[2].first};
	glm::vec3 max_bounds = {bounds[0].second, bounds[1].second, bounds[2].second};

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
	std::array<Vertex, 8> vertices = {
			Vertex{{min_bounds.x, min_bounds.y, min_bounds.z}, {0.0f, 0.0f}},
			Vertex{{min_bounds.x, max_bounds.y, min_bounds.z}, {0.0f, 1.0f}},
			Vertex{{max_bounds.x, max_bounds.y, min_bounds.z}, {1.0f, 1.0f}},
			Vertex{{max_bounds.x, min_bounds.y, min_bounds.z}, {1.0f, 0.0f}},
			Vertex{{min_bounds.x, min_bounds.y, max_bounds.z}, {0.0f, 0.0f}},
			Vertex{{min_bounds.x, max_bounds.y, max_bounds.z}, {0.0f, 1.0f}},
			Vertex{{max_bounds.x, max_bounds.y, max_bounds.z}, {1.0f, 1.0f}},
			Vertex{{max_bounds.x, min_bounds.y, max_bounds.z}, {1.0f, 0.0f}},
	};

	static_assert(sizeof(Vertex) == sizeof(glm::vec3) + sizeof(glm::vec2) &&
	              "Padding in struct");
	static_assert(sizeof(glm::vec3) == sizeof(float) * 3 && "Padding in vec3");
	static_assert(sizeof(glm::vec2) == sizeof(float) * 2 && "Padding in vec2");

	// 3 indices for triangle, 2 triangles for face, 6 faces
	// Counter-clockwise direction to see front face
	std::array<uint32_t, 3 * 2 * 6> raw_ibo = {
			0, 1, 3, 3, 1, 2,  // face 0-1-2-3
			0, 3, 4, 4, 3, 7,  // face 0-4-7-3
			0, 4, 1, 1, 4, 5,  // face 0-4-5-1
			6, 7, 2, 2, 7, 3,  // face 6-7-3-2
			6, 2, 5, 5, 2, 1,  // face 6-5-1-2
			6, 5, 7, 7, 5, 4   // face 6-7-4-5
	};

	auto layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(3);
	layout->Push<float>(2);
	auto vbo = VertexBuffer::Create(vertices, std::move(layout));

	auto ibo = IndexBuffer::Create(raw_ibo);

	vao_ = VertexArray::Create(std::move(vbo), std::move(ibo));
}
