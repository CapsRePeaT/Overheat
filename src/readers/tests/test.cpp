#include <gtest/gtest.h>

#include <fstream>

#include "../src/virtex_data_provider.h"
#include "../src/virtex_reader.h"

std::string sample =
		"VIRTEX\n"
		"35. 35.\n"
		"B\n"
		"0.4 0.05 1.\n"
		"4 \n"
		" 2.  2. 32  4\n"
		" 2.  6.  4 24\n"
		" 2. 30. 32  4\n"
		"30.  6.  4 24\n"
		"H\n"
		"0.36 1.E-3 0.\n"
		"0. 35. 0. 35.\n"
		"#\n"
		"P\n"
		"1\n"
		"1.0 0.01 0.2 0.2 0. 0.\n"
		"0. 35. 0. 35.\n"
		"S\n"
		"0.2 0.05 1.\n"
		"4\n"
		" 4.  4. 28  4\n"
		" 4.  8.  4 20\n"
		" 4. 28. 28  4\n"
		"28.  8.  4 20\n"
		"U\n"
		" 0.2 0.15 0.\n"
		" 2. 33. 2. 33.\n"
		"S\n"
		" 0.1 0.05 0.5\n"
		" 12\n"
		" 11.0 14.0 7  1\n"
		" 11.0 21.0 7  1\n"
		" 11.0 14.5 1 13\n"
		" 14.0 14.5 1 13\n"
		" 16.0 12.0 7  1\n"
		" 16.0 23.0 7  1\n"
		" 16.0 12.5 1 21\n"
		" 19.0 12.5 1 21 \n"
		" 21.0 12.0 7  1\n"
		" 21.0 23.0 7  1\n"
		" 21.0 12.5 1 21\n"
		" 24.0 12.5 1 21\n"
		"D\n"
		"0.2\n"
		"0.15 3\n"
		" 'GTZ1' 4.0 1\n"
		" 10.5 14.5 13.5 21.5\n"
		" 'SLR1' 8.0 1\n"
		" 15.5 19.5 11.5 23.5\n"
		" 'SLR2' 8.0 1\n"
		" 20.5 24.5 11.5 23.5\n"
		"#\n"
		"#\n"
		"#\n"
		"&AM TC=20. AK=0. &END\n"
		"&CL NH=30 MH=30 ITERAT=15000 EPS=1.E-6 W=1.99 &END";


TEST(VirtexReader, read_heat_test) {
  auto data = Readers::read_geometry(sample);
  fs::path trm = fs::current_path().parent_path().parent_path() / "readers/doc/test/virtex.TRM";
  fs::path t2d = fs::current_path();
  Readers::VirtexReader(trm, t2d);
  EXPECT_TRUE(true);
}

TEST(VirtexReader, read_raw_test) {
	auto data = Readers::read_geometry(sample);
	EXPECT_TRUE(true);
}

TEST(VirtexReader, read_geometry_test) {
	auto data = Readers::read_geometry(sample);
	auto geometry = Readers::VirtexDataProvider(data, {}).geometry();
	EXPECT_TRUE(true);
}

