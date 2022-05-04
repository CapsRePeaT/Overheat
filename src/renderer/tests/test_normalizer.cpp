#include <gtest/gtest.h>

#include <iterator>
#include <utility>
#include <vector>

#include "heatmap_normalizer.h"
#include "log.h"

namespace {

constexpr std::pair<float, float> trivial_coord_bounds = {0.0f, 1.0f};
constexpr std::pair<float, float> trivial_values       = {0.0f, 1.0f};

constexpr std::pair<float, float> basic_coord_bounds = {1.5f, 2.7f};
constexpr std::pair<float, float> basic_values       = {4.0, 21.7};

}  // namespace

TEST(TrivialCase, PlainLerp) {
	Log::Init();
	constexpr float interpolating_point_coord = 0.3f;

	const auto trivial = HeatmapNormalizer::Lerp(
			trivial_coord_bounds, trivial_values, interpolating_point_coord);
	ASSERT_FLOAT_EQ(trivial, interpolating_point_coord);
}

TEST(BasicCase, PlainLerp) {
	const auto basic_case =
			HeatmapNormalizer::Lerp(basic_coord_bounds, basic_values, 1.8);
	ASSERT_FLOAT_EQ(basic_case, 8.425f);
}

TEST(TrivialCase, BatchSize) {
	// result batch points should be {0.1, 0.7}, so result is 2
	const auto trivial = HeatmapNormalizer::CalculateBatchSize(
			trivial_coord_bounds.second, 0.1f, 0.6f);
	ASSERT_EQ(trivial, 2);
	const auto trivial_with_right = HeatmapNormalizer::CalculateBatchSize(trivial_coord_bounds.second, 0.0f, 0.5f);
	ASSERT_EQ(trivial_with_right, 3);
}

TEST(BasicCase, BatchSize) {
	// result is {1.8, 2.2, 2.6}
	const auto basic_case = HeatmapNormalizer::CalculateBatchSize(
			basic_coord_bounds.second, 1.8f, 0.4f);
	ASSERT_EQ(basic_case, 3);
}

TEST(TrivialCase, BatchLerp) {
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
	// Log::Init();
	std::vector<float> result;
	std::vector<float> assert_result = {0.0f, 0.6f, 1.4f, 2.6f, 3.4f, 4.0f, 5.2f, 5.8f, 5.2f, 4.6f, 4.0f};
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
TEST(BasicCase, FullInterpolation) {
	std::vector<float> temperatures = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
	                                   2.f, 2.f, 1.f, 0.f, 0.f, 2.f, 3.f, 3.f,
	                                   2.f, 0.f, 0.f, 2.f, 3.f, 4.f, 2.f, 0.f,
	                                   0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
	std::vector<float> x_steps      = {1.f, 3.f, 1.f, 3.f, 3.f};
	std::vector<float> y_steps      = {1.f, 4.f, 1.f, 2.f};
	float min_step{1.f};
	HeatmapNormalizer normalizer(min_step, 11.f, 8.f, 16, x_steps, y_steps);
	Heatmap h(temperatures, x_steps.size() + 1, y_steps.size() + 1);
	// h.DebugPrint();

	auto res_h = normalizer.BilinearInterpolate(h);
	// res_h.DebugPrint();
}

TEST(BasicCase, FullInterpolationOnesFirstRow) {
	Log::Init();
	
	std::vector<float> temperatures = {
		1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 
	};
	std::vector<float> x_steps = {1.f, 3.f, 1.f, 3.f, 3.f};
	float min_step{1.f};
	std::vector<float> result;
	HeatmapNormalizer::InterpolateRow(temperatures, x_steps, 11.f / 15, std::back_inserter(result));
	ASSERT_EQ(result.size(), 16);
	for (auto el : result) {
		ASSERT_FLOAT_EQ(el, 1.f);
	}
}

TEST(BasicCase, FullInterpolationOnes) {
	Log::Init();
	std::vector<float> temperatures = {
		1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 
		1.f, 2.f, 3.f, 3.f, 2.f, 1.f, 
		1.f, 3.f, 4.f, 4.f, 3.f, 1.f, 
		1.f, 3.f, 4.f, 5.f, 3.f, 1.f,
		1.f, 1.f, 1.f, 1.f, 1.f, 1.f
	};
	std::vector<float> x_steps      = {1.f, 3.f, 1.f, 3.f, 3.f};
	std::vector<float> y_steps      = {1.f, 4.f, 1.f, 2.f};
	float min_step{1.f};
	HeatmapNormalizer normalizer(min_step, 11.f, 8.f, 16, x_steps, y_steps);
	Heatmap h(temperatures, x_steps.size() + 1, y_steps.size() + 1);

	auto res_h = normalizer.BilinearInterpolate(h);
	h.DebugPrint();
	res_h.DebugPrint();
}
