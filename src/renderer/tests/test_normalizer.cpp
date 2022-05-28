#include <gtest/gtest.h>

#include <iterator>
#include <utility>
#include <vector>

#include "common.h"
#include "databases.h"
#include "heatmap.h"
#include "heatmap_normalizer.h"
#include "log.h"

namespace {

constexpr std::pair<float, float> trivial_coord_bounds = {0.0f, 1.0f};
constexpr std::pair<float, float> trivial_values       = {0.0f, 1.0f};

constexpr std::pair<float, float> basic_coord_bounds = {1.5f, 2.7f};
constexpr std::pair<float, float> basic_values       = {4.0, 21.7};

void LogInit() {
	static bool is_inited = false;
	if (!is_inited) {
		is_inited = true;
		Log::Init();
	}
}

}  // namespace

TEST(TrivialCase, PlainLerp) {
	LogInit();
	constexpr float interpolating_point_coord = 0.3f;

	const auto trivial = HeatmapNormalizer::Lerp(
			trivial_coord_bounds, trivial_values, interpolating_point_coord);
	ASSERT_FLOAT_EQ(trivial, interpolating_point_coord);
}

TEST(BasicCase, PlainLerp) {
	LogInit();
	const auto basic_case =
			HeatmapNormalizer::Lerp(basic_coord_bounds, basic_values, 1.8);
	ASSERT_FLOAT_EQ(basic_case, 8.425f);
}

TEST(TrivialCase, BatchSize) {
	LogInit();
	// result batch points should be {0.1, 0.7}, so result is 2
	const auto trivial = HeatmapNormalizer::CalculateBatchSize(
			trivial_coord_bounds.second, 0.1f, 0.6f);
	ASSERT_EQ(trivial, 2);
	const auto trivial_with_right = HeatmapNormalizer::CalculateBatchSize(
			trivial_coord_bounds.second, 0.0f, 0.5f);
	ASSERT_EQ(trivial_with_right, 3);
}

TEST(BasicCase, BatchSize) {
	LogInit();
	// result is {1.8, 2.2, 2.6}
	const auto basic_case = HeatmapNormalizer::CalculateBatchSize(
			basic_coord_bounds.second, 1.8f, 0.4f);
	ASSERT_EQ(basic_case, 3);
}

TEST(TrivialCase, BatchLerp) {
	LogInit();
	std::vector<float> result;
	std::vector<float> assert_result = {0.1f, 0.7f};
	HeatmapNormalizer::BatchLerp(trivial_coord_bounds, trivial_values, 0.1f, 0.6f,
	                             std::back_inserter(result));

	ASSERT_EQ(result.size(), assert_result.size());
	for (size_t i = 0; i < result.size(); ++i) {
		ASSERT_FLOAT_EQ(result[i], assert_result[i]);
	}
}

TEST(BasicCase, BatchLerp) {
	LogInit();
	std::vector<float> result;
	std::vector<float> assert_result = {8.425f, 14.325f, 20.225f};
	HeatmapNormalizer::BatchLerp(basic_coord_bounds, basic_values, 1.8, 0.4,
	                             std::back_inserter(result));

	ASSERT_EQ(result.size(), assert_result.size());
	for (size_t i = 0; i < result.size(); ++i) {
		ASSERT_FLOAT_EQ(result[i], assert_result[i]);
	}
}

TEST(TrivialCase, LerpRow) {
	LogInit();
	std::vector<float> result;
	std::vector<float> assert_result      = {0.0f, 0.6f, 1.4f, 2.6f, 3.4f, 4.0f,
                                      5.2f, 5.8f, 5.2f, 4.6f, 4.0f};
	std::vector<float> trivial_row_values = {0.0f, 1.0f, 3.0f, 4.0f, 6.0f, 4.0f};
	HeatmapNormalizer::InterpolateRow(trivial_row_values,
	                                  {1.0f, 1.0f, 1.0f, 1.0f, 2.0f}, 0.6f,
	                                  std::back_inserter(result));

	LOG_DEBUG("result: {}\nassert_result: {}", result, assert_result);
	ASSERT_EQ(result.size(), assert_result.size());
	for (size_t i = 0; i < result.size(); ++i) {
		ASSERT_FLOAT_EQ(result[i], assert_result[i]);
	}
}
// TEST(BasicCase, FullInterpolation) {
// std::vector<float> temperatures = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
//                                    2.f, 2.f, 1.f, 0.f, 0.f, 2.f, 3.f, 3.f,
//                                    2.f, 0.f, 0.f, 2.f, 3.f, 4.f, 2.f, 0.f,
//                                    0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
// std::vector<float> x_coords      = {1.f, 3.f, 1.f, 3.f, 3.f};
// std::vector<float> y_coords      = {1.f, 4.f, 1.f, 2.f};
// float min_step{1.f};
// HeatmapNormalizer normalizer(min_step, 11.f, 8.f, 16, x_coords, y_coords);
// Heatmap h(temperatures, x_coords.size() + 1, y_coords.size() + 1);
// h.DebugPrint();

// auto res_h = normalizer.BilinearInterpolate(h);
// res_h.DebugPrint();
// }

TEST(BasicCase, FullInterpolationOnesFirstRow) {
	LogInit();

	std::vector<float> temperatures = {
			1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
	};
	std::vector<float> x_steps = {1.f, 3.f, 1.f, 3.f, 3.f};
	float min_step{1.f};
	std::vector<float> result;
	HeatmapNormalizer::InterpolateRow(temperatures, x_steps, 11.f / 15,
	                                  std::back_inserter(result));
	ASSERT_EQ(result.size(), 16);
	for (auto el : result) {
		ASSERT_FLOAT_EQ(el, 1.f);
	}
}

TEST(BasicCase, FullInterpolationOnes) {
	LogInit();
	std::vector<float> temperatures = {
			2.f, 3.f, 3.f, 2.f,
			3.f, 4.f, 4.f, 3.f,
			3.f, 4.f, 5.f, 3.f,
	};
	std::vector<float> x_steps = {1.f, 3.f, 1.f, 3.f};
	std::vector<float> y_steps = {1.f, 4.f, 1.f};
	float min_step{1.f};
	HeatmapStorage storage(x_steps, y_steps, temperatures, 1.0f,
	                       Box3D({{0.0f, 10.f}, {0.0f, 8.0f}}));
	ASSERT_FLOAT_EQ(storage.MinStep(), min_step);
	HeatmapNormalizer normalizer(storage, 40);
	Heatmap h(temperatures, x_steps.size(), y_steps.size());

	auto res_h = normalizer.BilinearInterpolateSlow(h);
	h.DebugPrint();
	res_h.DebugPrint();
	normalizer.Normalize(res_h);
	res_h.DebugPrint();
}

void AssertCell(HeatmapNormalizer::Cell testing_cell,
                HeatmapNormalizer::Cell assert_cell) {
	ASSERT_FLOAT_EQ(testing_cell.x_bounds.first, assert_cell.x_bounds.first);
	ASSERT_FLOAT_EQ(testing_cell.x_bounds.second, assert_cell.x_bounds.second);
	ASSERT_FLOAT_EQ(testing_cell.y_bounds.first, assert_cell.y_bounds.first);
	ASSERT_FLOAT_EQ(testing_cell.y_bounds.second, assert_cell.y_bounds.second);

	ASSERT_FLOAT_EQ(testing_cell.upper_values.first,
	                assert_cell.upper_values.first);
	ASSERT_FLOAT_EQ(testing_cell.upper_values.second,
	                assert_cell.upper_values.second);
	ASSERT_FLOAT_EQ(testing_cell.lower_values.first,
	                assert_cell.lower_values.first);
	ASSERT_FLOAT_EQ(testing_cell.lower_values.second,
	                assert_cell.lower_values.second);
}

TEST(SlowTests, FindCellTest) {
	LogInit();
	std::vector<float> temperatures = {// 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
	                                   /*1.f,*/ 1.f, 2.f, 3.f,
	                                   /*1.f,*/ 4.f, 5.f, 6.f,
	                                  //  /*1.f,*/ 1.f, 1.f, 1.f
																		 };

	std::vector<float> x_coords = {0.0f, 1.f, 2.f, 3.f};
	std::vector<float> y_coords = {0.0f, 1.f, 2.f};
	Heatmap heatmap(temperatures, 4, 3);
	float env_temp = 0.0f;
	auto GetCell   = [&heatmap, &x_coords, &y_coords,
                  &env_temp](HeatmapNormalizer::FloatPair search_coords) {
    return HeatmapNormalizer::FindBilinearInterpolationCell(
					heatmap, x_coords, y_coords, search_coords, env_temp);
	};
	AssertCell(GetCell({1.5f, 1.5f}), {.x_bounds     = {1.f, 2.f},
	                                   .y_bounds     = {1.f, 2.f},
	                                   .upper_values = {1.f, 2.f},
	                                   .lower_values = {4.f, 5.f}});
	AssertCell(GetCell({2.5f, 1.5f}), {.x_bounds     = {2.f, 3.f},
	                                   .y_bounds     = {1.f, 2.f},
	                                   .upper_values = {2.f, 3.f},
	                                   .lower_values = {5.f, 6.f}});
}
