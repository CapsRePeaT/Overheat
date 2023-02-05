#include "solver3d_TRM.h"

#include <databases.h>
#include <shapes.h>

#include <cassert>
#include <ranges>
#include <cmath>

namespace {
GlobalId getNewShapeId() {
	static size_t id = 0;
	++id;
	// TODO: pass disign_id
	return {InstanceType::Shape, 0, id};
}
}  // namespace

namespace Readers::Solver3d {
LayerType BaseLayer::type() const { return type_; };

float BaseLayer::thermal_conductivity() const { return thermal_conductivity_; };

float BaseLayer::thickness() const { return thickness_; }
std::string_view BaseLayer::type_tag() const { return raw_type_tag_; };

std::istream& HU::read(std::istream& in) {
	in >> thickness_ >> thermal_conductivity_ >> env_thermal_conductivity_;
	in >> coordinates_.x1_ >> coordinates_.x2_ >> coordinates_.y1_ >>
			coordinates_.y2_;
	return in;
}

std::istream& P::read(std::istream& in) {
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
		in >> crystal.name >> crystal.power >> crystal.magic_number;
		in >> crystal.coordinates_.x1_ >> crystal.coordinates_.x2_ >>
				crystal.coordinates_.y1_ >> crystal.coordinates_.y2_;
		crystals_.push_back(crystal);
	}
	return in;
}

GeomStorage<BasicShape> HU::geometry() {
	Box3D box{{{coordinates_.x1_, coordinates_.x2_},
	           {coordinates_.y1_, coordinates_.y2_},
	           {0.f, thickness_}}};
	GeomStorage<BasicShape> storage;
	// assert(false && "add proper layer id and parent");
	const size_t dummy_layer = 0;
	storage.AddShape(
			std::make_shared<BasicShape>(getNewShapeId(), dummy_layer, box));
	return storage;
}

GeomStorage<BasicShape> P::geometry() {
	Box3D box{{{coordinates_.x1_, coordinates_.x2_},
	           {coordinates_.y1_, coordinates_.y2_},
	           {0.f, thickness_}}};
	GeomStorage<BasicShape> storage;
	// assert(false && "add proper layer id and parent");
	const size_t dummy_layer = 0;
	storage.AddShape(
			std::make_shared<BasicShape>(getNewShapeId(), dummy_layer, box));
	return storage;
}

GeomStorage<BasicShape> BS::geometry() {
	GeomStorage<BasicShape> storage;
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
				storage.AddShape(
						std::make_shared<BasicShape>(getNewShapeId(), dummy_layer, box));
				offset_y += dist_between_spheres_ ;
			}
			offset_x += dist_between_spheres_ ;
		}
	}
	return storage;
}
GeomStorage<BasicShape> D::geometry() {
	GeomStorage<BasicShape> storage;
	for (const auto& crystal : crystals_) {
		Box3D box{{{crystal.coordinates_.x1_, crystal.coordinates_.x2_},
		           {crystal.coordinates_.y1_, crystal.coordinates_.y2_},
		           {0.f, thickness_}}};
		// assert(false && "add proper layer id and parent");
		const size_t dummy_layer = 0;
		storage.AddShape(
				std::make_shared<BasicShape>(getNewShapeId(), dummy_layer, box));
	}
	return storage;
}

}  // namespace Readers::Solver3d
