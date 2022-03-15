#include "virtex_reader.h"

#include <magic_enum.hpp>

#include <fstream>
#include <regex>
#include <string_view>
#include <unordered_map>

#include "T2D.h"
#include "virtex_data_provider.h"

namespace {
std::string stream_to_string(std::ifstream& stream) {
	return std::string{std::istreambuf_iterator<char>(stream), {}};
}

std::vector<std::string> split(const std::string& str, const std::regex& regex,
                               int submatch = 0) {
	std::vector<std::string> retval{
			std::sregex_token_iterator(str.begin(), str.end(), regex, submatch), {}};

	retval.erase(std::remove_if(retval.begin(), retval.end(),
	                            [](const auto& str) { return str.empty(); }),
	             retval.end());
	return retval;
}

void read_general_info(const std::string& content, Readers::TRM& data) {
	std::stringstream istream;
	istream.str(content);
	istream >> data.program_name_;
	istream >> data.size_;
}

std::shared_ptr<Readers::BaseLayer> make_layer(std::string layer_tag) {
	layer_tag.erase(std::remove(layer_tag.begin(), layer_tag.end(), '\n'),
                  layer_tag.end());

	auto layer_type = magic_enum::enum_cast<Readers::LayerType>(layer_tag);
	if (!layer_type.has_value())
		throw std::runtime_error("Unknown layer type.");
	if (Readers::isHPU(*layer_type))
		return std::make_shared<Readers::HPU>(*layer_type);
	if (Readers::isBS(*layer_type))
		return std::make_shared<Readers::BS>(*layer_type);
	return std::make_shared<Readers::D>(*layer_type);
}

std::shared_ptr<Readers::BaseLayer> read_layer(
		const std::string& layer_tag, const std::string& layer_content) {
	std::stringstream ss{layer_content};
	auto layer = make_layer(layer_tag);
	layer->read(ss);
	return layer;
}

std::string validate_and_get_content(const fs::path& path) {
	std::ifstream ifs{path};
	if (!ifs.good()) {
		throw std::runtime_error("Stream state is not good");
	}
	auto content = stream_to_string(ifs);
	ifs.close();
	return content;
}

}  // namespace

namespace Readers {
void VirtexReader::load() {
	data_provider_ =
			std::make_unique<VirtexDataProvider>(load_geometry(), load_heatmap());
}
TRM VirtexReader::load_geometry() {
	const auto file_content = validate_and_get_content(trm_file_);
	return read_geometry(file_content);
}

// Function which reads files similar to doc/virtex.txt
TRM read_geometry(const std::string& content) {
	// splits layers for groups (inside/outside box)
	const auto groups =
			split(content, std::regex{"^([A-Za-z]\\n[^#]*(?=\\n#))$"});

	if (groups.size() < 2)
		throw std::runtime_error("File has wrong format.");

	TRM data;
	read_general_info(content, data);

	// read layers
	const std::regex layers_regex{"^([A-Z])\\n"};
	std::transform(
			groups.begin(), groups.end(),
			std::inserter(data.layers_groups_, data.layers_groups_.end()),
			[&layers_regex, index = 0](
					const auto& group) mutable -> std::pair<GroupsPosition, Layers> {
				const auto layers_tags    = split(group, layers_regex);
				const auto layers_content = split(group, layers_regex, -1);
				Layers layers{};
				for (size_t i = 0; i < layers_content.size(); ++i) {
					layers.push_back(read_layer(layers_tags[i], layers_content[i]));
				}
				return {(GroupsPosition)index++, layers};
			});

	return data;
}
T2D read_heatmap(std::ifstream& istream) {
	T2D t2d;
	istream >> t2d;
	return t2d;
}

T2D VirtexReader::load_heatmap() {
	std::ifstream ifs{t2d_file_};
	if (!ifs.good()) {
		throw std::runtime_error("Stream state is not good");
	}
	auto map = read_heatmap(ifs);
	ifs.close();
	return map;
}

}  // namespace Readers
