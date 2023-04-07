
#include <gtest/gtest.h>

#include <fstream>

#include "../src/solver3d/solver3d_data_provider.h"
#include "../src/solver3d/solver3d_reader.h"
#include "core.h"

TEST(Solver3d, test_cgal) {

	EXPECT_TRUE(true);
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
	core.CalculateHeat(rep, SolverSetup());

	EXPECT_TRUE(true);
}
