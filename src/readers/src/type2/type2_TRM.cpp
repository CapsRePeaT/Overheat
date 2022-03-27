#include "type2_TRM.h"

#include <magic_enum.hpp>

#include <any>
#include <regex>

using namespace magic_enum;

namespace {
void read_layers_info(std::istream& istream, Readers::Type2::LayerType& layer,
                      int layers_num) {
	if (layers_num > 0) {
		std::string tmp;
		istream >> layer.zc_.set_size(layers_num) >> tmp;
		istream >> layer.vc_.set_size(layers_num) >> tmp;
		istream >> layer.cc_.set_size(layers_num) >> tmp;
		istream >> layer.rc_.set_size(layers_num) >> tmp;
	}
}

template <typename Key, typename Val>
void init_vars_from_raw(std::unordered_map<Key, Val>& vars,
                        const std::string& raw_vars) {
	// split strings to variables names and values (VarName)=(VarValue)
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

namespace Readers::Type2 {
std::istream& operator>>(std::istream& istream, type2_TRM& trm) {
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

}  // namespace Readers::Type2