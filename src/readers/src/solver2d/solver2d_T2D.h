#pragma once
#include <istream>
#include <map>
#include <numeric>
#include <vector>

#include "../common.h"

struct Net {
	Box2D box{};

	friend std::istream& operator>>(std::istream& in, Net& net) {
		std::pair<float, float> x_ray;
		std::pair<float, float> y_ray;
		in >> x_ray.first >> x_ray.second >> y_ray.first >> y_ray.second;
		net.box = Box2D({x_ray, y_ray});
		return in;
	}
};

namespace {

enum Index { X = 0, Y = 1 };

std::vector<float> get_steps(const std::vector<Net>& net, const Index index) {
	std::vector<float> steps{};
	std::transform(net.begin(), net.end(), std::back_inserter(steps),
	               [index](const auto node) {
									 const auto coords = node.box.coordinates();
									 return coords[index].first - coords[index].second;
								 });
	return steps;
}
}  // namespace

// class with heatmap and data about layers
class Solver2d_T2D {
 public:
	[[nodiscard]] Box3D design_borders() const {
		// FIXME we don't have z coords in 2d geometry,
		// so put some default value
		return {{{0.f, xsub_}, {0.f, ysub_}, {0.f, 1.f}}};
	}
	[[nodiscard]] std::vector<float> net_x() const {
		return std::vector<float>(mo_, hx_);  // first&last filled externally
	}
	[[nodiscard]] std::vector<float> net_y() const {
		return std::vector<float>(no_, hy_);  // first&last filled externally
	}
	[[nodiscard]] const std::vector<float>& temperatures() const {
		return temperature_.data();
	}
	friend std::istream& operator>>(std::istream& istream, Solver2d_T2D& t2d) {
		istream >> t2d.program_name_;
		istream >> t2d.mo_ >> t2d.no_ >> t2d.mogr_ >> t2d.nogr_ >> t2d.isgr_ >>
				t2d.jsgr_ >> t2d.kkk_ >> t2d.nk_ >> t2d.m2_;
		istream >> t2d.xsub_ >> t2d.ysub_ >> t2d.hx_ >> t2d.hy_ >> t2d.hxgr_ >>
				t2d.hygr_;
		istream >> t2d.net_.set_size(t2d.m2_);
		istream >> t2d.ns_.set_size(t2d.m2_);
		istream >> t2d.ms_.set_size(t2d.kkk_);
		istream >> t2d.temperature_.set_size(t2d.mo_ * t2d.no_);
		return istream;
	}

 public:
	std::string program_name_{};

	// MO,NO - количество узлов по x и y, соответственно.
	// MOGR,NOGR - количество узлов по x и y, используемых для построения графиков
	// ISGR,JSGR - столько узлов исходной сетки пропускается, когда выбираются
	// узлы для построения графиков. Примечание: MOGR,NOGR,ISGR,JSGR
	// использовались в старой версии программы, в новой версии вводятся значения,
	// но не используются.
	// KKK - количество элементов схемы.
	// NK - количество изотерм.
	// M2 - общее количество прямоугольников во всех элементах схемы.
	size_t mo_, no_, mogr_, nogr_, isgr_, jsgr_, kkk_, nk_, m2_;

	// XSUB,YSUB - размеры кристалла по горизонтали.
	// HX,HY - шаги сетки по горизонтали.
	// Примечание: HXGR,HYGR - шаги сетки по горизонтали, используемые для
	// построения графиков. HXGR,HYGR использовались в старой версии программы
	float xsub_, ysub_, hx_, hy_, hxgr_, hygr_;

	// XL(I),XP(I) - минимальная и максимальная координаты прямоугольника по оси x
	// YS(I),YH(I) - минимальная и максимальная координаты прямоугольника по оси y
	SerializableVec<Net> net_{};

	// NS(I) - номер элемента ИС, в который входит прямоугольник.
	SerializableVec<size_t> ns_{};

	// MS(I) - номер последнего прямоугольника, входящего в I-й элемент ИС.
	SerializableVec<size_t> ms_{};

	//В следующих строках вводится действительный двумерный массив значений
	//температуры в узлах разностной сетки, массив вводится по столбцам,
	//разделений между столбцами нет:
	//	((TM(I,J),I=1,MO ),J=1,NO)
	// TM(I,J) - значения температуры в узлах разностной сетки
	//Примечание: в каждый источник может входить один или несколько
	//прямоугольников, номера прямоугольников, входящих в I-й источник: от
	// MS(I-1)+1 до MS(I).
	SerializableVec<float> temperature_{};
};
