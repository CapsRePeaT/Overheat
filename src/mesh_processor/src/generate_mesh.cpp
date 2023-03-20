#include "generate_mesh.h"

#include <boost/range/algorithm.hpp>
#include <cinolib/find_intersections.h>
#include <cinolib/profiler.h>
#include <cinolib/tetgen_wrap.h>

#include <execution>

#include "utils.h"

namespace MeshProcessor {
using namespace cinolib;

CustomTetmesh MeshGenerator::get_tetmesh(bool show_mesh) {
	Profiler profiler;
	profiler.push("mesh creation");

	auto tet_meshes = generate_layers_meshes(representation_.layers(), show_mesh);
	const auto heat_data = representation_.shapes_metadata();
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
			p_data.intensity_of_heat_source =
					shape_heat_data.power * tet_mesh.poly_volume(pid) / mesh_volume;
			p_data.corner_conditions = shape_heat_data.corner_conditions;
		}
		total_tetmesh += tet_mesh;
	}
	profiler.pop();
	return total_tetmesh;
}

TetmeshVec MeshGenerator::generate_layers_meshes(const LayersShapes& layers,
                                                 bool show_mesh) {
	LayersMehses layers_meshes;
	layers_meshes.reserve(layers.size());
	for (const auto& layer : layers) {
		LayerMehses layer_meshes;
		layer_meshes.reserve(layer.size());
		for (const auto& box : layer) {
			const auto& coords = box.bbox().coordinates();
			BoxMesh box_mesh(coords[0], coords[1], coords[2], box.layer_id(),
			                 area_constraint_, corner_points_step_);
			layer_meshes.push_back(box_mesh);
		}
		layers_meshes.push_back(layer_meshes);
	}

	auto meshes = generate_trimesh_from_layers(layers_meshes);
	auto tets   = generate_tetmesh_from_trimeshes(meshes, show_mesh);
	return tets;
}

TetmeshVec MeshGenerator::generate_tetmesh_from_trimeshes(TrimeshVec& meshes,
                                                          bool show_mesh) {
	// FOR DEBUG
	if (show_mesh) {
		cinolib::DrawableTrimesh<> temp_meshes;
		for (const auto& mesh : meshes) {
			temp_meshes += mesh;
		}
		using namespace cinolib;
		GLcanvas gui(1920, 980);
		temp_meshes.updateGL();
		DrawableArrow x(vec3d(-35000, 0, 0), vec3d(35000, 0, 0));
		x.color = Color::GREEN();
		x.size  = 30;
		DrawableArrow y(vec3d(0, -35000, 0), vec3d(0, 35000, 0));
		y.color = Color::BLUE();
		y.size  = 30;
		DrawableArrow z(vec3d(0, 0, -35000), vec3d(0, 0, 35000));
		z.size = 30;

		// for cutting geometry and look inside
		// MeshSlicer slicer;
		// slicer.X_thresh = 0.6f; // in percents
		// slicer.slice(mesh);
		// mesh.updateGL();
		gui.push(&temp_meshes);
		gui.push(&x);
		gui.push(&y);
		gui.push(&z);
		gui.launch();
	}

	TetmeshVec ret;
	ret.reserve(meshes.size());

	boost::transform(
			meshes, std::back_inserter(ret),
			[this](const auto& tri_mesh) { return generate_tetmesh(tri_mesh); });

	return ret;
}

CustomTetmesh MeshGenerator::generate_tetmesh(const DrawableTrimesh<>& mesh) {
	std::vector<uint> edges, tets;
	std::vector<double> verts;
	char opt[100];
	const auto bbox = mesh.bbox();
	const auto min_edge =
			corner_points_step_.has_value()
					? *corner_points_step_
					: std::min({bbox.delta_x(), bbox.delta_y(), bbox.delta_z()});
	const auto constraint = volume_constraint_(min_edge);
	sprintf(opt, "YQqa%f", constraint);
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
				for (auto vert : upper_mesh.xy.vector_verts())
					boundary_verts_[ConstraintSide::xy].erase(vert);
			}

			for (auto lower_box_id : mesh.boxes_lower) {
				auto& lower_mesh = layers[mesh.layer - 1][lower_box_id];
				mesh.xy += lower_mesh.xy_z;
				for (auto vert : lower_mesh.xy_z.vector_verts())
					boundary_verts_[ConstraintSide::xy_z].erase(vert);
			}

			mesh.merge_meshes();

			boundary_verts_[ConstraintSide::xy].insert(mesh.xy.vector_verts().begin(),
			                                           mesh.xy.vector_verts().end());
			boundary_verts_[ConstraintSide::yz].insert(mesh.yz.vector_verts().begin(),
			                                           mesh.yz.vector_verts().end());
			boundary_verts_[ConstraintSide::xz].insert(mesh.xz.vector_verts().begin(),
			                                           mesh.xz.vector_verts().end());
			boundary_verts_[ConstraintSide::xy_z].insert(
					mesh.xy_z.vector_verts().begin(), mesh.xy_z.vector_verts().end());
			boundary_verts_[ConstraintSide::xz_y].insert(
					mesh.xz_y.vector_verts().begin(), mesh.xz_y.vector_verts().end());
			boundary_verts_[ConstraintSide::yz_x].insert(
					mesh.yz_x.vector_verts().begin(), mesh.yz_x.vector_verts().end());

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
