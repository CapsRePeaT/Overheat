#pragma once

#include <array>
#include <istream>
#include <memory>
#include <vector>

#include "../../core/include/databases.h"
#include "../../core/include/shapes.h"

namespace Readers {

enum class LayerType { BS, HPU, D, UNDEFINED };  // layers types from Ryabov doc

struct HorizontalSize {
	float length;
	float width;

	friend std::istream& operator>>(std::istream& in, HorizontalSize& obj) {
		in >> obj.length >> obj.width;  // mb this order is wrong. TBD with Ryabov
		return in;
	}
};

struct Coordinates {
	float x1_;
	float x2_;
	float y1_;
	float y2_;
};

class BaseLayer {
 public:
	// getters
	LayerType getType();
	float getThermalConductivity();
	float getThickness();

	// read ryabov file content from stream
	virtual std::istream& read(std::istream& in) = 0;
	virtual GeomStorage<BasicShape> getGeometry() = 0;

	~BaseLayer() = default;

 protected:
	LayerType layer_type_ = LayerType::UNDEFINED;
	float thermal_conductivity_;
	float thickness_;
};

class HPU : public BaseLayer {
 public:
	HPU() { layer_type_ = LayerType::HPU; }

	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> getGeometry() override;

 private:
	float env_thermal_conductivity_;
  Coordinates coordinates_;
};

class BS : public BaseLayer {
 public:
	BS() { layer_type_ = LayerType::BS; }

	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> getGeometry() override;

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
	D() { layer_type_ = LayerType::D; }

	std::istream& read(std::istream& in) override;
	GeomStorage<BasicShape> getGeometry() override;

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

using Layers = std::vector<std::shared_ptr<BaseLayer>>;

struct VirtexData {
	std::string program_name_;
	HorizontalSize size_;
	Layers layers_;
};

}  // namespace Readers
