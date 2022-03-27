#include <gtest/gtest.h>

#include "../src/type2/type2_reader.h"


namespace {
std::string sample =
		" OPERATIONAL AMPLIFIER\n"
		" &CP LAYER2=3 NBB=1  /\n"
		"    1.    1.\n"
		" 300. 0. 0.\n"
		"    0.4  0.05  2.0              ZC2\n"
		"  0.155 3.1E-4 0.0162           VC2\n"
		"   710. 1670. 0.837E6           CC2\n"
		"2.33E-6 1.2E-6 8.3E-6          RC2\n"
		"&PRT XLP=0.5,XPP=1.,YSP=0.5,YPP=1. /\n"
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

TEST(type2, read_geometry) {
  Readers::Type2::read_geometry(sample);
	EXPECT_TRUE(true);
}
