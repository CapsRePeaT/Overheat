#include "type2_reader.h"


#include <fstream>
#include <regex>
#include <string_view>

#include "type2_data_provider.h"

namespace {}  // namespace

namespace Readers::Type2 {
type2_TRM read_geometry(std::string& content) {
	// the rest part of file is not needed as documentation says
	if (auto content_end_tag = content.rfind("\n' ' 0 0");
	    content_end_tag != std::string::npos)
		content.resize(content_end_tag);
	else
		throw std::runtime_error("Wrong file format.");
	content.shrink_to_fit();

	type2_TRM trm;
	std::stringstream ss{content};

	ss >> trm;
	return trm;
}

void Type2Reader::load() {
	data_provider_ = std::make_unique<Type2DataProvider>(load_geometry());
}
type2_TRM Type2Reader::load_geometry() {
	auto content = validate_and_get_content(trm_file_);
	return read_geometry(content);
}

}  // namespace Readers::Type2
