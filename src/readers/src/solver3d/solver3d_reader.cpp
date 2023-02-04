#include "solver3d_reader.h"

#include <boost/regex.hpp>
#include <magic_enum.hpp>

#include <fstream>
#include <string_view>
#include <unordered_map>

#include "solver3d_data_provider.h"

namespace {

std::vector<std::string> split(const std::string& str,
                               const boost::regex& regex, int submatch = 0) {
	std::vector<std::string> retval{
			boost::sregex_token_iterator(str.begin(), str.end(), regex, submatch),
			{}};

	retval.erase(std::remove_if(retval.begin(), retval.end(),
	                            [](const auto& str) { return str.empty(); }),
	             retval.end());
	return retval;
}

void read_general_info(const std::string& content,
                       Readers::Solver3d::Solver3d_TRM& data) {
	std::stringstream istream;
	istream.str(content);
	istream >> data.program_name_;
	istream >> data.size_;
}

std::shared_ptr<Readers::Solver3d::BaseLayer> make_layer(
		std::string layer_tag) {
	layer_tag.erase(std::remove(layer_tag.begin(), layer_tag.end(), '\n'),
	                layer_tag.end());

	auto layer_type =
			magic_enum::enum_cast<Readers::Solver3d::LayerType>(layer_tag);
	if (!layer_type.has_value())
		throw std::runtime_error("Unknown layer type.");
	if (Readers::Solver3d::isHU(*layer_type))
		return std::make_shared<Readers::Solver3d::HU>(*layer_type);
	if (Readers::Solver3d::isP(*layer_type))
		return std::make_shared<Readers::Solver3d::P>(*layer_type);
	if (Readers::Solver3d::isBS(*layer_type))
		return std::make_shared<Readers::Solver3d::BS>(*layer_type);
	return std::make_shared<Readers::Solver3d::D>(*layer_type);
}

std::shared_ptr<Readers::Solver3d::BaseLayer> read_layer(
		const std::string& layer_tag, const std::string& layer_content) {
	std::stringstream ss{layer_content};
	auto layer = make_layer(layer_tag);
	layer->read(ss);
	return layer;
}

}  // namespace

namespace Readers::Solver3d {
void Solver3dReader::load() {
	data_provider_ =
			std::make_unique<Solver3dDataProvider>(load_geometry(), load_heatmap());
}
Solver3d_TRM Solver3dReader::load_geometry() {
	const auto file_content = validate_and_get_content(trm_file_);
	return read_geometry(file_content);
}

// Function which reads files similar to doc/virtex.txt
Solver3d_TRM read_geometry(const std::string& content) {
	// splits layers for groups (inside/outside box)
	const auto groups =
			split(content, boost::regex{"^([A-Za-z]\\n[^#]*(?=\\n#))$"});

	if (groups.size() < 2)
		throw std::runtime_error("File has wrong format.");

	Solver3d_TRM data;
	read_general_info(content, data);

	// read layers
	const boost::regex layers_regex{"^([A-Z])\\n"};
	std::transform(
			groups.begin(), groups.end(),
			std::inserter(data.layers_groups_, data.layers_groups_.end()),
			[&layers_regex, index = 0](
					const auto& group) mutable -> std::pair<GroupsPosition, Layers> {
				const auto layers_tags = split(group, layers_regex);
				const auto layers_content = split(group, layers_regex, -1);
				Layers layers{};
				for (size_t i = 0; i < layers_content.size(); ++i) {
					layers.push_back(read_layer(layers_tags[i], layers_content[i]));
				}
				return {(GroupsPosition)index++, layers};
			});

	return data;
}

Solver3d_T2D read_heatmap(std::ifstream& istream) {
	Solver3d_T2D t2d;
	istream >> t2d;
	return t2d;
}

Solver3d_T2D Solver3dReader::load_heatmap() {
	std::ifstream ifs{t2d_file_};
	if (!ifs.good()) {
		throw std::runtime_error("Stream state is not good");
	}
	auto map = read_heatmap(ifs);
	ifs.close();
	return map;
}

}  // namespace Readers::Solver3d
