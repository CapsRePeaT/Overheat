#include "generate_mesh.h"

#include <boost/range/algorithm.hpp>
#include <cinolib/find_intersections.h>
#include <cinolib/profiler.h>
#include <cinolib/tetgen_wrap.h>

#include <execution>

#include "utils.h"

namespace {
bool operator>(const cinolib::vec3d& lhs, const cinolib::vec3d& rhs) {
	return std::tie(lhs.x(), lhs.y(), lhs.z()) >
	       std::tie(rhs.x(), rhs.y(), rhs.z());
}
}  // namespace

namespace MeshProcessor {
using namespace cinolib;

CustomTetmesh MeshGenerator::get_tetmesh() {
	Profiler profiler;
	profiler.push("mesh creation");

	auto tet_meshes = generate_layers_meshes(representation_.layers());
	const auto heat_data  = representation_.shapes_metadata();
	assert(heat_data.size() == tet_meshes.size() &&
	       "Heat data and shapes mismatch");
	CustomTetmesh total_tetmesh;
	for (auto i = 0; i < tet_meshes.size(); ++i) {
		const auto shape_heat_data = heat_data.at(i);
		auto& tet_mesh             = tet_meshes.at(i);
		auto mesh_volume           = tet_mesh.mesh_volume();
		auto& polys                = tet_mesh.vector_polys();
		for (auto pid = 0; pid < polys.size(); ++pid) {
			auto& p_data                = tet_mesh.poly_data(pid);
			p_data.thermal_conductivity = shape_heat_data.thermal_conductivity;
			p_data.ambient_temperature  = shape_heat_data.ambient_temperature;
			p_data.heat_flow            = shape_heat_data.heat_flow;
			p_data.intensity_of_heat_source =
					shape_heat_data.power * tet_mesh.poly_volume(pid) / mesh_volume;
			p_data.convective_heat = shape_heat_data.convective_heat;
		}
		total_tetmesh += tet_mesh;
	}
	profiler.pop();
	return total_tetmesh;
}

TetmeshVec MeshGenerator::generate_layers_meshes(const LayersShapes& layers) {
	LayersMehses layers_meshes;
	layers_meshes.reserve(layers.size());
	for (const auto& layer : layers) {
		LayerMehses layer_meshes;
		layer_meshes.reserve(layer.size());
		for (const auto& box : layer) {
			const auto& coords = box.bbox().coordinates();
			BoxMesh box_mesh(coords[0], coords[1], coords[2], box.layer_id(),
			                 area_thresh_);
			layer_meshes.push_back(box_mesh);
		}
		layers_meshes.push_back(layer_meshes);
	}

	auto meshes = generate_trimesh_from_layers(layers_meshes);
	auto tets   = generate_tetmesh_from_trimeshes(meshes);
	return tets;
}

TetmeshVec MeshGenerator::generate_tetmesh_from_trimeshes(TrimeshVec& meshes) {
	TetmeshVec ret;
	ret.reserve(meshes.size());

	boost::transform(meshes, std::back_inserter(ret), [this](const auto& tri_mesh) {
		return generate_tetmesh(tri_mesh);
	});

	return ret;
}

CustomTetmesh MeshGenerator::generate_tetmesh(const DrawableTrimesh<>& mesh) {
	std::vector<uint> edges, tets;
	std::vector<double> verts;
	char opt[100];
	sprintf(opt, "YQqa%f", volume_thresh_);
	tetgen_wrap(serialized_xyz_from_vec3d(mesh.vector_verts()),
	            serialized_vids_from_polys(mesh.vector_polys()), edges, opt,
	            verts, tets);

	return CustomTetmesh(verts, tets);
}

TrimeshVec MeshGenerator::generate_trimesh_from_layers(LayersMehses& layers) {
	calculate_holes_for_boxes(layers);
	calculate_mesh_and_translate_to_origin_pos(layers);

	TrimeshVec ret;
	for (auto& layer : layers) {
		for (auto& mesh : layer) {
			for (auto upper_box_id : mesh.boxes_upper) {
				auto& upper_mesh = layers[mesh.layer + 1][upper_box_id];
				mesh.xy_z += upper_mesh.xy;
			}
			for (auto lower_box_id : mesh.boxes_lower) {
				auto& lower_mesh = layers[mesh.layer - 1][lower_box_id];
				mesh.xy += lower_mesh.xy_z;
			}
			mesh.merge_meshes();
			ret.push_back(mesh.total_mesh);
		}
	}
	return ret;
}

void MeshGenerator::calculate_mesh_and_translate_to_origin_pos(
		LayersMehses& layers) {
	for (auto& layer : layers)
		for (auto& mesh : layer) {
			mesh.calculate_mesh();
			mesh.translate_to_origin();
		}
}

void MeshGenerator::calculate_holes_for_boxes(LayersMehses& layers) {
	for (int upper_ind = 1; upper_ind < layers.size(); ++upper_ind) {
		const auto lower_ind = upper_ind - 1;

		auto& lower_layers = layers[lower_ind];
		auto& upper_layers = layers[upper_ind];
		for (int u_ind = 0; u_ind < upper_layers.size(); ++u_ind) {
			auto& box_upper = upper_layers[u_ind];
			for (int l_ind = 0; l_ind < lower_layers.size(); ++l_ind) {
				auto& box_lower = lower_layers[l_ind];
				if (box_upper.min_point > box_lower.min_point &&
				    box_lower.max_point > box_upper.max_point) {
					// case when we should add hole to lower box
					//     |****|
					// |************|
					box_lower.boxes_upper.push_back(u_ind);

					auto ring  = box_upper.generator.xy_ring();
					auto shift = box_upper.min_point - box_lower.min_point;
					for (auto& vert : ring) vert += shift;
					box_lower.holes_upper.push_back(ring);
				} else if (box_upper.min_point < box_lower.min_point &&
				           box_lower.max_point < box_upper.max_point) {
					// case when we should add hole to upper box
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
}  // namespace MeshProcessor