#include "solver3d_TRM.h"

#include <databases.h>
#include <shapes.h>

#include <cassert>
#include <cmath>
#include <ranges>

namespace Readers::Solver3d {

std::istream& HU::read(std::istream& in) {
	corner_conditions_ = read_corner_conditions(in);
	in >> thickness_ >> thermal_conductivity_ >> env_thermal_conductivity_;
	in >> coordinates_.x1_ >> coordinates_.x2_ >> coordinates_.y1_ >>
			coordinates_.y2_;
	return in;
}

std::istream& P::read(std::istream& in) {
	corner_conditions_ = read_corner_conditions(in);
	in >> type_;
	in >> thickness_ >> thermal_conductivity_ >> border_thickness_ >>
			cup_thickness_ >> env_thermal_conductivity_ >>
			inner_thermal_conductivity_;
	in >> coordinates_.x1_ >> coordinates_.x2_ >> coordinates_.y1_ >>
			coordinates_.y2_;
	return in;
}

std::istream& BS::read(std::istream& in) {
	// thickness_ equals to spheres diameter
	corner_conditions_ = read_corner_conditions(in);
	in >> thickness_ >> thermal_conductivity_ >> dist_between_spheres_;
	size_t sphere_nums = 0;
	in >> sphere_nums;
	spheres_holders_.reserve(sphere_nums);
	for (size_t i = 0; i < sphere_nums; ++i) {
		SpheresHolders holder{};
		in >> holder.x_center >> holder.y_center >> holder.nx >> holder.ny;
		spheres_holders_.push_back(holder);
	}
	return in;
}

std::istream& D::read(std::istream& in) {
	in >> thickness_;
	in >> thermal_conductivity_;
	in >> crystals_num_per_layer_;

	crystals_.reserve(crystals_num_per_layer_);
	while (!in.eof()) {
		Crystal crystal;
		crystal.corner_conditions = read_corner_conditions(in);
		in >> crystal.name >> crystal.power >> crystal.magic_number;
		in >> crystal.coordinates.x1_ >> crystal.coordinates.x2_ >>
				crystal.coordinates.y1_ >> crystal.coordinates.y2_;
		crystals_.push_back(crystal);
	}
	return in;
}

ShapeHeatDataVec HU::shape_data() {
	Box3D box{{{coordinates_.x1_, coordinates_.x2_},
	           {coordinates_.y1_, coordinates_.y2_},
	           {0.f, thickness_}}};
	// assert(false && "add proper layer id and parent");
	const size_t dummy_layer = 0;
	BasicShape shape(getNewShapeId(), dummy_layer, box);
	ShapeHeatData heat_data{thermal_conductivity_, env_thermal_conductivity_, 0,
	                        corner_conditions_};
	return {{heat_data, shape}};
}

ShapeHeatDataVec P::shape_data() {
	Box3D box{{{coordinates_.x1_, coordinates_.x2_},
	           {coordinates_.y1_, coordinates_.y2_},
	           {0.f, thickness_}}};
	// assert(false && "add proper layer id and parent");
	const size_t dummy_layer = 0;
	BasicShape shape(getNewShapeId(), dummy_layer, box);
	ShapeHeatData heat_data{thermal_conductivity_, env_thermal_conductivity_, 0,
	                        corner_conditions_};

	return {{heat_data, shape}};
}

ShapeHeatDataVec BS::shape_data() {
	ShapeHeatDataVec storage;
	// thickness_ aka diameter for this case
	const auto radius = thickness_ / 2;

	for (const auto [x_center, y_center, nx, ny] : spheres_holders_) {
		float offset_x = 0;
		for (auto nx : std::views::iota(0, nx)) {
			float offset_y = 0;
			// const float x_1 = ;
			// const float x_2 = x_center + radius + offset_x;
			const auto x_ray = std::make_pair(x_center - radius + offset_x,
			                                  x_center + radius + offset_x);
			for (auto ny : std::views::iota(0, ny)) {
				// const float y_1 = y_center - radius + offset_y;
				// const float y_2 = ;
				const auto y_ray = std::make_pair(y_center - radius + offset_y,
				                                  y_center + radius + offset_y);
				Box3D::Values vals{x_ray, y_ray, {0.f, thickness_}};
				Box3D box{vals};
				// assert(false && "add proper layer id and parent");
				const size_t dummy_layer = 0;
				BasicShape shape(getNewShapeId(), dummy_layer, box);
				ShapeHeatData heat_data{thermal_conductivity_, 0, 0,
				                        corner_conditions_};
				storage.push_back({heat_data, shape});
				offset_y += dist_between_spheres_;
			}
			offset_x += dist_between_spheres_;
		}
	}
	return storage;
}
ShapeHeatDataVec D::shape_data() {
	ShapeHeatDataVec storage;
	for (const auto& crystal : crystals_) {
		Box3D box{{{crystal.coordinates.x1_, crystal.coordinates.x2_},
		           {crystal.coordinates.y1_, crystal.coordinates.y2_},
		           {0.f, thickness_}}};
		// assert(false && "add proper layer id and parent");
		const size_t dummy_layer = 0;
		BasicShape shape(getNewShapeId(), dummy_layer, box);
		ShapeHeatData heat_data{thermal_conductivity_, 0, crystal.power,
		                        corner_conditions_};
		storage.push_back({heat_data, shape});
	}
	return storage;
}

void read_condition(std::istream& in, CornerCondition& condition) {
	std::string temp_name;
	in >> temp_name >> condition.heat_flow >> condition.convective_heat >>
			condition.temperature;

	auto greater_zero_condition_count = 0;
	for (auto constraint : {condition.heat_flow, condition.convective_heat,
	                        condition.temperature}) {
		if (constraint > 0)
			++greater_zero_condition_count;
	}
	if (greater_zero_condition_count > 1)
		throw std::runtime_error("To much constraints");
}

CornerConditions BaseLayer::read_corner_conditions(std::istream& in) {
	CornerConditions conditions;
	read_condition(in, conditions.xy);
	read_condition(in, conditions.xy_z);
	read_condition(in, conditions.xz);
	read_condition(in, conditions.xz_y);
	read_condition(in, conditions.yz);
	read_condition(in, conditions.yz_x);
	return conditions;
}
}  // namespace Readers::Solver3d
