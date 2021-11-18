#include "virtex_reader.h"

#include <fstream>
#include <regex>
#include <string_view>

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

void read_general_info(const std::string& content, VirtexData& data) {
	std::stringstream istream;
	istream.str(content);
	istream >> data.program_name_;
	istream >> data.size_;
}

std::shared_ptr<BaseLayer> make_layer(const std::string& layer_tag) {
	switch (std::toupper(layer_tag[0])) {
		case 'H':
		case 'P':
		case 'U': {
			return std::make_shared<HPU>();
		}
		case 'B':
		case 'S': {
			return std::make_shared<BS>();
		}
		case 'D':
			return std::make_shared<D>();
		default:
			throw std::runtime_error("Unknown layer type.");
	}
}

std::shared_ptr<BaseLayer> read_layer(const std::string& layer_tag,
                                      const std::string& layer_content) {
	std::stringstream ss{layer_content};
	auto layer = make_layer(layer_tag);
	layer->read(ss);
	return layer;
}
}  // namespace

namespace Readers {
void VirtexReader::load() {
	std::ifstream ifs{file_};
	if (!ifs.good()) {
		throw std::runtime_error("Stream state is not good.");
	}
	const auto file_content = stream_to_string(ifs);
	const auto data = read(file_content);
	ifs.close();
	data_provider_ = std::make_unique<VirtexDataProvider>(data);
}

// Function which reads files similar to doc/virtex.txt
VirtexData read(const std::string& content) {
	// splits layers for groups (inside/outside box)
	const auto groups =
			split(content, std::regex{"^([A-Za-z]\\n[^#]*(?=\\n#))$"});

	if (groups.size() < 2) throw std::runtime_error("File has wrong format.");

	VirtexData data;
	read_general_info(content, data);

	// read layers
	for (const auto& group : groups) {
		// splits layers groups for particular layers
		const std::regex layers_regex{"^([A-Z])\\n"};
		const auto layers_tags = split(group, layers_regex);
		const auto layers_content = split(group, layers_regex, -1);
		// with boost it may looked better...
		for (size_t i = 0; i < layers_content.size(); ++i) {
			const auto layer = read_layer(layers_tags[i], layers_content[i]);
			data.layers_.push_back(layer);
		}
	}
	return data;
}
}  // namespace Readers
