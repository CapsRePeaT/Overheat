#include <gtest/gtest.h>

#include "../src/solver2d/solver2d_reader.h"

namespace {
std::string sample =
		" OPERATIONAL AMPLIFIER\n"
		" &CP LAYER2=3 M=6 N=6 NBB=1  /\n"
		"    1.    1.\n"
		" 300. 0. 0.\n"
		"    0.4  0.05  2.0              ZC2\n"
		"  0.155 3.1E-4 0.0162           VC2\n"
		"   710. 1670. 0.837E6           CC2\n"
		" 2.33E-6 1.2E-6 8.3E-6          RC2\n"
		" &PRT /\n"
		"'T1' 2.E-4 1\n"
		" .15 .20 .57 .60\n"
		"'T2' 2.E-4 1\n"
		" .15 .20 .40 .43\n"
		"'T3' 2.E-1 1\n"
		" .30 .40 .65 .85\n"
		"'T4' 5.E-2 1\n"
		" .70 .85 .55 .85\n"
		"'R'  3.E-3 1\n"
		" .55 .60 .15 .65\n"
		"' ' 0 0                         END OF LAYOUT\n"
		"                                END OF DATA";

}

TEST(Solver2d, read_file) {
	fs::path trm = fs::current_path().parent_path().parent_path() / "src" /
	               "readers" / "tests" / "en9.trm";
	fs::path t2d = fs::current_path().parent_path().parent_path() / "src" /
	               "readers" / "tests" / "en9.T2D";
	auto trm_ex = exists(trm);
	auto t2d_ex = exists(t2d);
	Readers::Solver2d::Solver2dReader(trm, t2d);
	EXPECT_TRUE(true);
}

TEST(Solver2d, read_geometry) {
	Readers::Solver2d::read_geometry(sample);
	EXPECT_TRUE(true);
}