#pragma once
#include <istream>
#include <numeric>
#include <vector>

#include <map>

namespace {
template <typename T>
class SerializableVec {
 public:
	SerializableVec() = default;

	SerializableVec& set_size(const size_t size) {
		elem_count_ = size;
		data_.reserve(elem_count_);
		return *this;
	}

	friend std::istream& operator>>(std::istream& in, SerializableVec& vec) {
		for (size_t i = 0; i < vec.elem_count_; ++i) {
			T value;
			in >> value;
			vec.data_.push_back(value);
		}
		return in;
	}
	[[nodiscard]] const std::vector<T>& data() const { return data_; }

 private:
	size_t elem_count_ = 0;
	std::vector<T> data_;
};

struct BodySize {
	float x, y, z;  //размеры корпуса по осям x, y, z:

	friend std::istream& operator>>(std::istream& in, BodySize& size) {
		in >> size.x >> size.y >> size.z;
		return in;
	}
};

struct BodyParams {
	float ZP;  // – высота бокса,
	float TP;  // – толщина стенок бокса,
	float TL;  // – толщина крышки бокса,
	float BP;  // – z-координата дна бокса

	friend std::istream& operator>>(std::istream& in, BodyParams& params) {
		in >> params.ZP >> params.TP >> params.TL >> params.BP;
		return in;
	}
};

struct Point2 {
	float x;
	float y;

	friend std::istream& operator>>(std::istream& in, Point2& coords) {
		in >> coords.x >> coords.y;
		return in;
	}
};

struct CoordsMinMax {
	Point2 min;
	Point2 max;

	friend std::istream& operator>>(std::istream& in, CoordsMinMax& coords) {
		in >> coords.min >> coords.max;
		return in;
	}
};

struct Spheres {
	explicit Spheres(const size_t count) {
		spheres_centers = SerializableVec<Point2>{}.set_size(count);
	}
	float diameter = 0;
	SerializableVec<Point2> spheres_centers;

	friend std::istream& operator>>(std::istream& in, Spheres& spheres) {
		in >> spheres.diameter;
		in >> spheres.spheres_centers;
		return in;
	}
};

struct SpheresGroup {
	SpheresGroup& init(const std::vector<size_t>& groups_spheres_count) {
		groups_spheres_count_ = groups_spheres_count;
		return *this;
	}

	friend std::istream& operator>>(std::istream& in, SpheresGroup& spheres) {
		for (auto count : spheres.groups_spheres_count_) {
			Spheres spheres_lauer{count};
			in >> spheres_lauer;
			spheres.spheres_groups_.push_back(spheres_lauer);
		}
		return in;
	}
	std::vector<size_t> groups_spheres_count_;
	std::vector<Spheres> spheres_groups_;
};
}  // namespace

// class with heatmap and data about layers
class T2D {
 public:
	size_t layers_count() const { return IST_; }
	const std::vector<float>& net_x() const { return net_x_coords.data(); }
	const std::vector<float>& net_y() const { return net_y_coords.data(); }
	const std::vector<float>& temperatures() const { return temperature_.data(); }
	friend std::istream& operator>>(std::istream& istream, T2D& t2d) {
		istream >> t2d.program_name_;
		istream >> t2d.body_size_;
		istream >> t2d.IST_;
		istream >> t2d.IH_;
		istream >> t2d.ID_;
		istream >> t2d.IB_;
		istream >> t2d.IV_;
		istream >> t2d.IP_;
		istream >> t2d.NH_;
		istream >> t2d.MH_;
		istream >> t2d.raw_layers_;
		istream >> t2d.num_crystals_in_crystal_layer_;
		istream >> t2d.common_layer_num_crystals_;
		istream >> t2d.box_num_;
		istream >> t2d.spheres_count_.set_size(t2d.IB_);
		istream >> t2d.common_layesr_num_spheres_.set_size(t2d.IB_);
		istream >> t2d.spheres_out_layers_.set_size(t2d.IB_);
		istream >> t2d.body_id_;
		istream >> t2d.body_params_;
		istream >> t2d.body_coords_;
		if (t2d.IH_ > 0)
			istream >> t2d.ihs_.set_size(t2d.IST_);
		istream >> t2d.temp_.set_size(t2d.IST_);
		istream >> t2d.ids_.set_size(t2d.num_crystals_in_crystal_layer_);
		istream >> t2d.spheres_.init(t2d.spheres_count_.data());
		istream >> t2d.net_y_coords.set_size(t2d.MH_);
		istream >> t2d.net_x_coords.set_size(t2d.NH_);
		istream >> t2d.min_max_layers_coords_.set_size(t2d.IST_ * 2 * 2);
		istream >> t2d.thicknesses_.set_size(t2d.IST_);
		istream >> t2d.topology_.set_size(t2d.IST_ * t2d.NH_ * t2d.MH_);
		istream >> t2d.env_temp_;
		istream >> t2d.temperature_.set_size(t2d.IST_ * t2d.NH_ * t2d.MH_);
		

		//float total_x_step = 0;
		//for (const float net_x_coord : t2d.net_x_coords.data())
		//	total_x_step += net_x_coord; 
		//float total_y_step = 0;
		//for (const float net_y_coord : t2d.net_y_coords.data())
		//	total_y_step += net_y_coord; 
		return istream;
	}

 private:
	//имя программы
	std::string program_name_;
	//размер бокса
	BodySize body_size_;
	size_t IST_;  //– общее число слоев в корпусе,
	size_t IH_;   // – число однородных слоев,
	size_t ID_;   // – число слоев кристаллов ИС,
	size_t IB_;   // – число слоев шариковых выводов,
	size_t IV_;   // – число слоев межсоединений
	size_t IP_;   // – число боксов
	size_t NH_;   // – число узлов сетки по оси x,
	size_t MH_;   // – число узлов сетки по оси y
	std::string raw_layers_;  // string with char representation of layers
	size_t num_crystals_in_crystal_layer_;  //количество кристаллов в слое
	                                        //кристаллов,
	size_t common_layer_num_crystals_;  // общие номера слоев кристалов
	size_t box_num_;  // вводится номер бокса, в котором лежит данный слой
	                  // кристаллов:
	SerializableVec<size_t> spheres_count_;  // количество шариковых выводов
	                                         // в I-м слое шариковых выводов
	                                         // IB>0
	SerializableVec<size_t>
			common_layesr_num_spheres_;  // общие номера слоев кристалов шариковых
	SerializableVec<size_t> spheres_out_layers_;  //номер бокса, в котором лежит
	                                              //данный слой шариковых выводов
	size_t body_id_;            //номер бокса
	BodyParams body_params_;    //параметры бокса
	CoordsMinMax body_coords_;  //координаты бокса
	//*Если IH>0, вводятся данные об однородных слоях:
	//В строке 14 вводится номер бокса, в котором лежит этот слой, номер
	//однородного слоя, это его номер в общей нумерации слоев: (LH(I),I=1,IST)
	// 0 0 0 1 0 0 1 0
	//В строке 15 вводится, но не используется (KH(I),I=1,IST)
	// 0 0 0 0 0 0 1 1*//
	SerializableVec<size_t> ihs_;
	SerializableVec<size_t> temp_;  // not used
	/*Далее вводятся ID групп координат кристаллов ИС в ID слоях кристаллов:
	(XL(J,I),XP(J,I),YS(J,I),YH(J,I),I=1,KD(J))
	1.0500000E+01 1.4500000E+01 1.3500000E+01 2.1500000E+01 1.5500000E+01
	1.9500000E+01 1.1500000E+01 2.3500000E+01 2.0500000E+01 2.4500000E+01
	1.1500000E+01 2.3500000E+01
	XL(J,I),XP(J,I) – минимальная и максимальная координата кристалла по оси x,
	YS(J,I),YH(J,I) – минимальная и максимальная координата кристалла по оси y.
	(В этом примере 1 слой кристаллов и в нем 3 кристалла)
	*/
	SerializableVec<CoordsMinMax> ids_;
	/*Если IB>0, вводятся IB групп строк, описывающих шариковые выводы:
	В первой строке вводится диаметр шариков данного слоя: DB(J)
	*/
	SpheresGroup spheres_{spheres_count_.data()};  // IB>0
	//Далее вводятся массивы шагов разностной сетки по осям x и y:
	SerializableVec<float> net_x_coords;
	SerializableVec<float> net_y_coords;
	//Далее вводятся IST массивов минимальных и максимальных координат слоев по
	//горизонтали:
	SerializableVec<float> min_max_layers_coords_;
	//Далее вводится массив толщин слоев:
	SerializableVec<float> thicknesses_;
	//Далее вводятся IST массивов целого типа, кодирующих топологию слоев (K –
	//номер слоя)3
	SerializableVec<float> topology_;
	//Далее вводится температура окружающей среды:
	float env_temp_;
	//Далее вводятся IST массивов действительного типа значений температуры в
	//узлах сетки на границах раздела слоев (K – номер слоя)
	SerializableVec<float> temperature_;
};
