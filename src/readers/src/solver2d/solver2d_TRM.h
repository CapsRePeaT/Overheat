#pragma once

#include <array>
#include <iostream>
#include <istream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../../../core/include/databases.h"
#include "../../../core/include/shapes.h"
#include "../common.h"

namespace Readers::Solver2d {

//CP because this enum describes vars from such strings:
// &СP NBAUND=2,M=6,N=6,NBB=2,LAYER1=2,LAYER2=3
enum CPVars { NBAUND, M, N, NBB, LAYER1, LAYER2 };

struct CP {
	std::unordered_map<CPVars, int> values{{NBAUND, 0}, {M, 0},      {N, 0},
	                                       {NBB, 0},    {LAYER1, 0}, {LAYER2, 0}};
};

//PRT because this enum describes vars from such strings:
//&PRT XLP=0.5,XPP=1.,YSP=0.5,YPP=1. /
enum PRTVars { PR2, XLP, XPP, YSP, YHP,YPP };

struct PRT {
	std::unordered_map<PRTVars, float> values{
			{PR2, 0.f}, {XLP, 0.f}, {XPP, 0.f}, {YSP, 0.f}, {YHP, 0.f}, {YPP, 0.f}};
};

struct LayerType {
	SerializableVec<float> zc_;
	SerializableVec<float> vc_;
	SerializableVec<float> cc_;
	SerializableVec<float> rc_;

	void set_size(const size_t size) {
		zc_.set_size(size);
		vc_.set_size(size);
		cc_.set_size(size);
		rc_.set_size(size);
	}
};

struct Element {
	std::string sg_;
	float xff_;
	size_t nss_;
	Coordinates box_;

	friend std::istream& operator>>(std::istream& istream, Element& elem) {
		istream >> elem.sg_ >> elem.xff_ >> elem.nss_;
		for (auto i = 0; i < elem.nss_; ++i)
			istream >> elem.box_.x1_ >> elem.box_.x2_ >> elem.box_.y1_ >>
					elem.box_.y2_;
		return istream;
	}
};

struct Solver2d_TRM {
	std::string program_name_;
	std::string raw_cp_;
	CP cp_;
	HorizontalSize size_;
	float tc_;  //температура окружающей среды или корпуса, К
	float fc1_;  //коэффициент конвективного теплобмена сверху корпуса
	float fc2_;  //коэффициент конвективного теплобмена снизу корпуса
	LayerType l2_;
	LayerType l1_;
	std::string raw_prt_;
	PRT prt_;
	std::vector<Element> elements_;

	friend std::istream& operator>>(std::istream& istream, Solver2d_TRM& trm);
};

}  // namespace Readers::Type2
