#pragma once
enum class LayerType { BS, HPU, D, UNDEFINED };  // layers types from Ryabov doc

struct HorizontalSize {
	float length;
	float width;

	friend std::istream& operator>>(std::istream& in, HorizontalSize& obj) {
		in >> obj.length >> obj.width;  // mb this order is wrong. TBD with Ryabov
		return in;
	}
};

struct BaseLayer {
	virtual std::istream& read(std::istream& in) = 0;
	LayerType layer_type_ = LayerType::UNDEFINED;
	float thermal_conductivity_;
	float thickness_;

	~BaseLayer() = default;
};

struct HPU : public BaseLayer {
	HPU() { layer_type_ = LayerType::HPU; }

	float env_thermal_conductivity_;
	float x1_;
	float x2_;
	float y1_;
	float y2_;

	std::istream& read(std::istream& in) override {
		in >> thickness_ >> thermal_conductivity_ >> env_thermal_conductivity_;
		in >> x1_ >> x2_ >> y1_ >> y2_;
		return in;
	}
};

struct BS : public BaseLayer {
	BS() { layer_type_ = LayerType::BS; }
	float dist_between_spheres_;
	struct SpheresHolders {
		// coordinates of center of initial sphere
		float x_center;
		float y_center;
		int nx;  // num of spheres around X axis
		int ny;  // num of spheres around Y axis
	};
	std::vector<SpheresHolders> spheres_holders_;

	std::istream& read(std::istream& in) override {
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
};

struct D : public BaseLayer {
 public:
	D() { layer_type_ = LayerType::D; }

	struct Crystal {
		std::string name;
		float power;
		float magic_number;  // TODO: TBD with Ryabov
		std::array<float, 4> position;
	};

	size_t crystals_num_per_layer_;
	std::vector<Crystal> crystals_;

	std::istream& read(std::istream& in) override {
		in >> thickness_;
		in >> thermal_conductivity_;
		in >> crystals_num_per_layer_;

		crystals_.reserve(crystals_num_per_layer_);
		while (!in.eof()) {
			Crystal crystal;
			in >> crystal.name >> crystal.power >> crystal.magic_number;
			in >> crystal.position[0] >> crystal.position[1] >> crystal.position[2] >>
					crystal.position[3];
			crystals_.push_back(crystal);
		}
		return in;
	}
};

using Layers = std::vector<std::shared_ptr<BaseLayer>>;

struct VirtexData {
	std::string program_name_;
	HorizontalSize size_;
	Layers layers_;
};