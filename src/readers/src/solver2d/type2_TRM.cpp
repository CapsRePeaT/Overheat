#include <magic_enum.hpp>

#include <any>
#include <regex>

#include "solver2d_TRM.h"

using namespace magic_enum;

namespace {
void read_layers_info(std::istream& istream,
                      Readers::Solver2d::LayerType& layer,
                      const size_t layers_num) {
	if (layers_num) {
		std::string tmp;
		layer.set_size(layers_num);
		istream >> layer.zc_ >> tmp;
		istream >> layer.vc_ >> tmp;
		istream >> layer.cc_ >> tmp;
		istream >> layer.rc_ >> tmp;
	}
}

template <typename Key, typename Val>
void init_vars_from_raw(std::unordered_map<Key, Val>& vars,
                        const std::string& raw_vars) {
	// split strings to variables names and values (VarName)=(VarValue)
	// example: &СP NBAUND=2,M=6,N=6,NBB=2,LAYER1=2,LAYER2=3
	std::regex rgx(R"rgx(([a-zA-z1-9]+)[=\s]+([[0-9]*[.]*]*[0-9]+))rgx",
	               std::regex::extended);
	std::smatch match;
	bool keep_search = true;
	auto suffix      = raw_vars;
	while (keep_search) {
		keep_search = std::regex_search(suffix, match, rgx);
		if (keep_search) {
			if (auto variable_name = enum_cast<Key>(match[1].str())) {
				auto variable_value  = std::stof(match[2].str());
				vars[*variable_name] = variable_value;
				suffix               = match.suffix();
			} else
				throw std::runtime_error("Unknown variable type");
		}
	}
}

}  // namespace

namespace Readers::Solver2d {
std::istream& operator>>(std::istream& istream, Solver2d_TRM& trm) {
	std::getline(istream, trm.program_name_);

	std::getline(istream, trm.raw_cp_);
	init_vars_from_raw(trm.cp_.values, trm.raw_cp_);

	istream >> trm.size_;
	istream >> trm.tc_ >> trm.fc1_ >> trm.fc2_;

	read_layers_info(istream, trm.l2_, trm.cp_.values[CPVars::LAYER2]);
	read_layers_info(istream, trm.l1_, trm.cp_.values[CPVars::LAYER1]);

	std::getline(istream, trm.raw_prt_);
	std::getline(istream, trm.raw_prt_);
	init_vars_from_raw(trm.prt_.values, trm.raw_prt_);

	while (!istream.eof()) {
		Element element;
		istream >> element;
		trm.elements_.push_back(element);
	}
	return istream;
};

}  // namespace Readers::Solver2d
