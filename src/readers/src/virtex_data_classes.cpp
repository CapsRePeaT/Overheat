#include "virtex_data_classes.h"

#include <databases.h>
#include <shapes.h>

#include <ranges>
#include <cassert>

namespace {
size_t getNewId() {
	static size_t id{0};
	++id;
	return id;
}

std::pair<float, float> getSphereProjection(const float center,
                                            const size_t offset,
                                            const float radius) {
	return {center + offset - radius, center + offset + radius};
}
}  // namespace

namespace Readers {
LayerType BaseLayer::type() { return type_; };

float BaseLayer::thermal_conductivity() { return thermal_conductivity_; };

float BaseLayer::thickness() { return thickness_; };

std::istream& HPU::read(std::istream& in) {
	in >> thickness_ >> thermal_conductivity_ >> env_thermal_conductivity_;
	in >> coordinates_.x1_ >> coordinates_.x2_ >> coordinates_.y1_ >>
			coordinates_.y2_;
	return in;
}

std::istream& BS::read(std::istream& in) {
	// thickness_ equals to spheres diameter
	in >> thickness_ >> thermal_conductivity_ >> dist_between_spheres_;
	size_t sphere_nums;
	in >> sphere_nums;
	spheres_holders_.reserve(sphere_nums);
	for (auto i = 0; i < sphere_nums; ++i) {
		SpheresHolders holder;
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

GeomStorage<BasicShape> HPU::geometry() {
	Box3D box{{{coordinates_.x1_, coordinates_.x2_},
	           {coordinates_.y1_, coordinates_.y2_},
	           {0.f, thickness_}}};
	GeomStorage<BasicShape> storage;
	//assert(false && "add proper layer id and parent");
	const size_t dummy = 0;
	storage.AddShape(std::make_shared<BasicShape>(getNewId(), box, dummy));
	return storage;
}

GeomStorage<BasicShape> BS::geometry() {
	GeomStorage<BasicShape> storage;
	// thickness_ aka diameter for this case
	const auto radius = thickness_ / 2;

	for (const auto [x_center, y_center, nx, ny] : spheres_holders_) {
		for (auto nx : std::views::iota(0, nx)) {
			const auto x_ray = getSphereProjection(x_center, nx, radius);
			for (auto ny : std::views::iota(0, ny)) {
				const auto y_ray = getSphereProjection(y_center, ny, radius);
				Box3D::Values vals{x_ray, y_ray, {0.f, thickness_}};
				Box3D box{vals};
				//assert(false && "add proper layer id and parent");
				const size_t dummy = 0;
				storage.AddShape(std::make_shared<BasicShape>(getNewId(), box, dummy));
			}
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
		//assert(false && "add proper layer id and parent");
		const size_t dummy = 0;
		storage.AddShape(std::make_shared<BasicShape>(getNewId(), box, dummy));
	}
	return storage;
}

}  // namespace Readers
