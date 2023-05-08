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

inline GlobalId getNewShapeId(bool reset = false, size_t new_rep_id = 0) {
	static size_t id = 0;
	static size_t rep_id = 0;
	++id;
	if (reset)
		id = 0;
	if (new_rep_id != 0)
		rep_id = new_rep_id;
	// TODO: pass disign_id
	return { InstanceType::Shape, 0, id };
}

using ShapeHeatDataVec = std::vector<std::pair<ShapeHeatData, BasicShape>>;
class BaseLayer {
 public:
	explicit BaseLayer(LayerType type) : type_(type) {}
	// getters
	[[nodiscard]] LayerType type() const { return type_; };
	[[nodiscard]] std::string_view type_tag() const { return raw_type_tag_; };
	[[nodiscard]] float thickness() const { return thickness_; };
	[[nodiscard]] CornerConditions read_corner_conditions(std::istream& in);

	// read ryabov file content from stream
	virtual std::istream& read(std::istream& in) = 0;
	virtual ShapeHeatDataVec shape_data()        = 0;

	~BaseLayer() = default;

 protected:
	std::string raw_type_tag_{};
	LayerType type_ = LayerType::UNDEFINED;
	float thermal_conductivity_{};
	float thickness_{};
	CornerConditions corner_conditions_;
};

class HU : public BaseLayer {
 public:
	explicit HU(LayerType type) : BaseLayer(type){};
	std::istream& read(std::istream& in) override;
	ShapeHeatDataVec shape_data() override;

 private:
	float env_thermal_conductivity_{};
	Coordinates coordinates_{};
};

class P : public BaseLayer {
 public:
	explicit P(LayerType type) : BaseLayer(type){};
	std::istream& read(std::istream& in) override;
	ShapeHeatDataVec shape_data() override;

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
	ShapeHeatDataVec shape_data() override;

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
	ShapeHeatDataVec shape_data() override;

 private:
	struct Crystal {
		std::string name;
		float power;
		float magic_number;  // TODO: TBD with Ryabov
		Coordinates coordinates;
		CornerConditions corner_conditions;
	};

	size_t crystals_num_per_layer_;
	std::vector<Crystal> crystals_;
};

enum GroupsPosition { UnderBody, Body, AboveBody };

using Layers       = std::vector<std::shared_ptr<BaseLayer>>;
using LayersGroups = std::map<GroupsPosition, Layers>;

struct Solver3d_TRM_Metadata {
	double env_temperature;
	double cup_temp_cond;
};

struct Solver3d_TRM {
	std::string program_name_;
	HorizontalSize size_;
	LayersGroups layers_groups_;
	Solver3d_TRM_Metadata metadata_;
};

}  // namespace Readers::Solver3d
