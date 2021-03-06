#include "solver2d_reader.h"

#include <fstream>
#include <regex>
#include <string_view>

#include "solver2d_data_provider.h"

namespace Readers::Solver2d {
Solver2d_TRM read_geometry(std::string& content) {
	// the rest part of file is not needed as documentation says
	if (auto content_end_tag = content.rfind("\n' ' 0 0");
	    content_end_tag != std::string::npos)
		content.resize(content_end_tag);
	else
		throw std::runtime_error("Wrong file format.");
	content.shrink_to_fit();

	Solver2d_TRM trm;
	std::stringstream ss{content};

	ss >> trm;
	return trm;
}

Solver2d_T2D read_heatmap(const std::string& content) {
	Solver2d_T2D t2d;
	std::stringstream ss{content};
	ss >> t2d;
	return t2d;
}

void Solver2dReader::load() {
	data_provider_ =
			std::make_unique<Solver2dDataProvider>(load_geometry(), load_heatmap());
}
Solver2d_TRM Solver2dReader::load_geometry() {
	auto content = validate_and_get_content(trm_file_);
	return read_geometry(content);
}

Solver2d_T2D Solver2dReader::load_heatmap() {
	auto content = validate_and_get_content(t2d_file_);
	return read_heatmap(content);
}

}  // namespace Readers::Solver2d
