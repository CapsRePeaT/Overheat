#include "generate_mesh.h"

#include <cinolib/find_intersections.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/profiler.h>
#include <cinolib/tetgen_wrap.h>

#include <iostream>

#include "box_mesh.h"
#include "utils.h"

using LayerMehses  = std::vector<MeshProcessor::BoxMesh>;
using LayersMehses = std::vector<LayerMehses>;

using TrimeshVec = std::vector<cinolib::DrawableTrimesh<>>;
using TetmeshVec = std::vector<cinolib::DrawableTetmesh<>>;

namespace {

bool operator>(const cinolib::vec3d& lhs, const cinolib::vec3d& rhs) {
	return std::tie(lhs.x(), lhs.y(), lhs.z()) >
	       std::tie(rhs.x(), rhs.y(), rhs.z());
}

void calculate_holes_for_boxes(LayersMehses& layers) {
	for (int upper_ind = 1; upper_ind < layers.size(); ++upper_ind) {
		auto lower_ind = upper_ind - 1;

		auto& lower_layers = layers[lower_ind];
		auto& upper_layers = layers[upper_ind];
		for (int u_ind = 0; u_ind < upper_layers.size(); ++u_ind) {
			auto& box_upper = upper_layers[u_ind];
			for (int l_ind = 0; l_ind < lower_layers.size(); ++l_ind) {
				auto& box_lower = lower_layers[l_ind];
				if (box_upper.min_point > box_lower.min_point &&
				    box_lower.max_point > box_upper.max_point) {
					//     |****|
					// |************|
					box_lower.boxes_upper.push_back(u_ind);

					auto ring  = box_upper.generator.xy_ring();
					auto shift = box_upper.min_point - box_lower.min_point;
					for (auto& vert : ring) vert += shift;
					box_lower.holes_upper.push_back(ring);
				} else if (box_upper.min_point < box_lower.min_point &&
				           box_lower.max_point < box_upper.max_point) {
					// |************|
					//     |****|
					box_upper.boxes_lower.push_back(l_ind);

					auto ring  = box_lower.generator.xy_z_ring();
					auto shift = box_lower.min_point - box_upper.min_point;
					for (auto& vert : ring) vert += shift;
					box_upper.holes_lower.push_back(ring);
				}
			}
		}
	}
}

void calculate_mesh_and_translate_to_origin_pos(LayersMehses& layers) {
	using namespace cinolib;
	static int counter = 0;
	for (auto& layer : layers) {
		for (auto& mesh : layer) {
			mesh.calculate_mesh();
			mesh.translate_to_origin();
			++counter;
		}
	}
}

TrimeshVec generate_trimesh_from_layers(LayersMehses& layers) {
	cinolib::Profiler profiler;
	profiler.push("Calculate holes and meshes");
	calculate_holes_for_boxes(layers);
	calculate_mesh_and_translate_to_origin_pos(layers);

	// add meshes to upper holes
	// add meshes to lower holes
	TrimeshVec ret;
	for (auto& layer : layers) {
		for (auto& mesh : layer) {
			for (auto upper_box_id : mesh.boxes_upper) {
				auto& upper_mesh = layers[mesh.layer + 1][upper_box_id];
				// mesh.xy_z += upper_mesh.xy;
			}
			for (auto lower_box_id : mesh.boxes_lower) {
				auto& lower_mesh = layers[mesh.layer - 1][lower_box_id];
				// mesh.xy += lower_mesh.xy_z;
			}
			mesh.merge_meshes();
			ret.push_back(mesh.total_mesh);
		}
	}
	profiler.pop();
	return ret;
}

cinolib::DrawableTetmesh<> generate_tetmesh(
		const cinolib::DrawableTrimesh<>& mesh) {
	static int i = 0;
	std::cout << "generate_tetmesh: " << i << std::endl;

	std::vector<uint> edges, tets;
	std::vector<double> verts;
	double vol_thresh = 50000;
	char opt[100];
	sprintf(opt, "Yd");
	auto serialized_verts =
			cinolib::serialized_xyz_from_vec3d(mesh.vector_verts());
	auto serialized_polys =
			cinolib::serialized_vids_from_polys(mesh.vector_polys());
	cinolib::tetgen_wrap(serialized_verts, serialized_polys, edges, opt, verts,
	                     tets);
	++i;
	return cinolib::DrawableTetmesh<>(verts, tets);
}

TetmeshVec generate_tetmesh_from_trimeshes(TrimeshVec& meshes) {
	TetmeshVec ret;
	ret.reserve(meshes.size());
	cinolib::Profiler profiler;
	profiler.push("generate_tetmesh_from_trimeshes");
	static int count = 0;
	for (const auto& tri_mesh : meshes) {
		ret.push_back(generate_tetmesh(tri_mesh));
		std::cout << "tetgen count:" << count << std::endl;
		++count;
	}
	profiler.pop();
	return ret;
}
}  // namespace

namespace MeshProcessor {
void generate(const LayersShapes& layers) {
	LayersMehses layers_meshes;
	layers_meshes.reserve(layers.size());
	for (auto& layer : layers) {
		LayerMehses layer_meshes;
		layer_meshes.reserve(layer.size());
		for (auto& box : layer) {
			const auto& coords = box.bbox().coordinates();
			MeshProcessor::BoxMesh box_mesh(coords[0], coords[1], coords[2],
			                                box.layer_id());
			layer_meshes.push_back(box_mesh);
		}
		layers_meshes.push_back(layer_meshes);
	}

	auto meshes = generate_trimesh_from_layers(layers_meshes);

	GLcanvas gui(1920, 980);
	auto& mesh     = meshes[494];
	auto& points_m = mesh.vector_verts();

	std::vector<vec3d> duplicated_verts;
	for (size_t i = 0; i < points_m.size(); ++i) {
		for (size_t j = i + 1; j < points_m.size(); ++j) {
			if (points_m[i] == points_m[j]) {
				duplicated_verts.emplace_back(points_m[i]);
			}
		}
	}

	DrawableArrow x(vec3d(-35000, 0, 0), vec3d(35000, 0, 0));
	x.color = Color::GREEN();
	x.size  = 10;
	DrawableArrow y(vec3d(0, -35000, 0), vec3d(0, 35000, 0));
	y.color = Color::BLUE();
	y.size  = 10;
	DrawableArrow z(vec3d(0, 0, -35000), vec3d(0, 0, 35000));
	z.size = 10;

	gui.push(&x);
	gui.push(&y);
	gui.push(&z);

	std::set<ipair> inters;
	find_intersections(mesh, inters);
	std::cout << "\n"
						<< inters.size() << " pairs of intersecting triangles were found\n"
						<< std::endl;

	for (const auto& i : inters) {
		mesh.poly_data(i.first).color  = Color::GREEN();
		mesh.poly_data(i.second).color = Color::YELLOW();
		// mesh.poly_remove(i.second);
		mesh.poly_data(i.first).flags[MARKED]  = true;
		mesh.poly_data(i.second).flags[MARKED] = true;
	}

	mesh.updateGL();
	gui.push(&mesh);
	gui.launch();

	auto tets = generate_tetmesh_from_trimeshes(meshes);
}
}  // namespace MeshProcessor