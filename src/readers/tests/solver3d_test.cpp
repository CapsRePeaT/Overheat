
#include <cinolib/geodesics.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/profiler.h>
#include <gtest/gtest.h>

#include <fstream>

#include "../src/solver3d/solver3d_data_provider.h"
#include "../src/solver3d/solver3d_reader.h"
#include "core.h"
TEST(Solver3d, test_cgal) {
	using namespace cinolib;

	std::string s = std::string(
											"C:"
											"\\Users\\asavinov\\dev\\miem\\Overheat\\lib\\cinolib\\ex"
											"amples\\data") +
	                "/bunny.obj";
	DrawableTrimesh<> m(s.c_str());
	m.show_wireframe(false);

	GLcanvas gui;
	SurfaceMeshControls<DrawableTrimesh<>> menu(&m, &gui);
	gui.push(&m);
	gui.push(&menu);

	// position heat sources with CMD + left click
	Profiler profiler;
	std::vector<uint> sources;
	GeodesicsCache prefactored_matrices;

	uint vid = 2;
	sources.push_back(vid);
	profiler.push("compute_geodesics");
	auto num = m.num_verts();
	auto geo =compute_geodesics_amortized(m, prefactored_matrices, sources);
	geo.copy_to_mesh(m);
	profiler.pop();
	m.show_texture1D(TEXTURE_1D_HSV_W_ISOLINES);

	//gui.launch();
}

TEST(Solver3d, read_heat_test) {
	fs::path trm = fs::current_path().parent_path().parent_path() / "src" /
	               "readers" / "tests" / "virtex.TRM";
	fs::path t2d = fs::current_path().parent_path().parent_path() / "src" /
	               "readers" / "tests" / "virtex.t2d";
	auto trm_ex = exists(trm);
	auto t2d_ex = exists(t2d);

	auto& core = Core::instance();
	auto id    = core.LoadRepresentationWithHeatmap(trm.string(), t2d.string(),
	                                                GeometryType::D3);
	auto& rep  = core.GetRepresentation(id);
	core.CalculateHeat(rep);

	EXPECT_TRUE(true);
}
