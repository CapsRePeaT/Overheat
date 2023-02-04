#pragma once

#include <array>
#include <istream>
#include <map>
#include <memory>
#include <vector>

#include "../common.h"
#include "databases.h"
#include "shapes.h"

namespace Readers::Solver3d {

// HPU - boxes
// BS - spheres
enum class LayerType {
	B,
	S,
	H,
	P,
	U,
	D,
	UNDEFINED
};  // layers types from Ryabov doc

inline bool isHU(LayerType type) {
	return type == LayerType::H || type == LayerType::U;
}

inline bool isP(LayerType type) { return type == LayerType::P; }

inline bool isBS(LayerType type) {
	return type == LayerType::B || type == LayerType::S;
}

class BaseLayer {
 public:
	explicit BaseLayer(LayerType type) : type_(type) {}
	// getters
	[[nodiscard]] LayerType type() const;
	[[nodiscard]] std::string_view type_tag() const;
	[[nodiscard]] float thermal_conductivity() const;
	[[nodiscard]] float thickness() const;

	// read ryabov file content from stream
	virtual std::istream& read(std::istream& in) = 0;
	virtual GeomStorage<BasicShape> geometry() = 0;

	~BaseLayer() = default;

 protected:
	std::string raw_type_tag_{};
	LayerType type_ = LayerType::UNDEFINED;
	float thermal_conductivity_{};
	float thickness_{};
};

class HU : public BaseLayer {
 public:
	explicit HU(LayerType type) : BaseLayer(type){};
	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> geometry() override;

 private:
	float env_thermal_conductivity_{};
	Coordinates coordinates_{};
};

class P : public BaseLayer {
 public:
	explicit P(LayerType type) : BaseLayer(type){};
	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> geometry() override;

 private:
	size_t type_;
	float env_thermal_conductivity_{};
	float border_thickness_{};
	float cup_thickness_{};
	float inner_thermal_conductivity_{};
	Coordinates coordinates_{};
};

class BS : public BaseLayer {
 public:
	BS(LayerType type) : BaseLayer(type){};
	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> geometry() override;

 private:
	float dist_between_spheres_;
	struct SpheresHolders {
		// coordinates of center of initial sphere
		float x_center;
		float y_center;
		int nx;  // num of spheres around X axis
		int ny;  // num of spheres around Y axis
	};
	std::vector<SpheresHolders> spheres_holders_;
};

class D : public BaseLayer {
 public:
	D(LayerType type) : BaseLayer(type){};
	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> geometry() override;

 private:
	struct Crystal {
		std::string name;
		float power;
		float magic_number;  // TODO: TBD with Ryabov
		Coordinates coordinates_;
	};

	size_t crystals_num_per_layer_;
	std::vector<Crystal> crystals_;
};

enum GroupsPosition { UnderBody, Body, AboveBody };

using Layers = std::vector<std::shared_ptr<BaseLayer>>;
using LayersGroups = std::map<GroupsPosition, Layers>;

struct Solver3d_TRM {
	std::string program_name_;
	HorizontalSize size_;
	LayersGroups layers_groups_;
};

}  // namespace Readers::Solver3d
