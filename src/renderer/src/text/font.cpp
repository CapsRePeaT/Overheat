#include "font.h"

#include <array>
#include <filesystem>
#include <fstream>

#include "config.h"
#include "constants.h"
#include "log.h"
#include "renderer/renderer_api.h"


// #define STB_RECT_PACK_IMPLEMENTATION
// #include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
// #define STBTT_malloc(x, u) ((void)(u), new char[(x)])
// #define STBTT_free(x, u) ((void)(u), delete[] (x))
#include "stb_truetype.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace fs = std::filesystem;

namespace renderer {

struct Font::Impl {
	std::vector<stbtt_packedchar> stb_packed_chars_;
};

namespace {

std::filesystem::path GetFontPath(std::string_view font_name) {
	fs::path font_filename = font_name;
	if (!font_filename.has_extension() ||
	    font_filename.extension() != Font::font_extension) {
		font_filename.replace_extension(Font::font_extension);
	}
	return Config::instance().executable_dir_path() / "res" / "fonts" /
	       font_filename;
}

Font::FontData ReadFontData(std::filesystem::path font_path) {
	constexpr size_t one_megabyte = 1 << 20;
	const auto file_size          = fs::file_size(font_path);
	if (file_size > one_megabyte) {
		LOG_CRITICAL("Font file is too big ({}). Consider to increase buffer size",
		             file_size);
	}
	// I don't like that vector default-initializes with resize, but in rare
	// initializations it is acceptable
	Font::FontData font_buffer(one_megabyte);
	std::ifstream font_file_stream;
	font_file_stream.open(font_path, std::ios::in | std::ios::binary);
	font_file_stream.read(reinterpret_cast<char*>(font_buffer.data()),
	                      font_buffer.size());

	font_buffer.resize(font_file_stream.gcount());
	return font_buffer;
};

size_t GetIndexByCharCode(char32_t code) {
	if (code >= consts::ascii_base &&
	    code < consts::ascii_base + consts::ascii_num)
		return code - consts::ascii_base;
	if (code >= consts::cyrillic_base &&
	    code < consts::cyrillic_base + consts::cyrillic_num)
		return code - consts::cyrillic_base + consts::ascii_num;
	return -1;
}

}  // namespace

Font::Font(std::string font_name, int font_size)
		: font_size_(font_size),
			font_name_(std::move(font_name)),
			stb_font_ctx_(std::make_unique<stbtt_fontinfo>()),
			data_(std::make_unique<Impl>()) {}

Font::~Font() = default;

bool Font::Init() {
	if (is_inited_) return has_errors_;
	is_inited_ = true;
	LOG_INFO("Trying to load \"{}\" font...", font_name_);
	auto font_path = GetFontPath(font_name_);
	font_buffer_   = ReadFontData(font_path);
	if (!InitPackedAtlas())
		return has_errors_ = false;

	if (!stbtt_InitFont(stb_font_ctx_.get(), font_buffer_.data(), 0)) {
		LOG_ERROR("Font can't be inited");
		return has_errors_ = false;
	}

	atlas_texture_ = RendererAPI::factory().NewTexture2D(
			buffer_width_, buffer_height_, atlas_buffer_.data(), 1, Format::UBYTE);
	material_ = std::make_unique<FontMaterial>(atlas_texture_);

	return has_errors_ = true;
}

std::shared_ptr<Text2D> Font::CreateText(std::u32string text,
                                         glm::vec2 position,
                                         const bool align_to_pixels) {
	auto it = text_cache_.find(text);
	if (it != text_cache_.end())
		return (*it).second;

	auto text2d = std::make_shared<Text2D>();
	text2d->BeginPlacing(std::move(text), position, align_to_pixels);
	for (const auto c : text2d->text()) {
		const auto i = GetIndexByCharCode(c);
		if (i < 0)
			continue;
		stbtt_aligned_quad quad;
		stbtt_GetPackedQuad(data_->stb_packed_chars_.data(), buffer_width_, buffer_height_, i,
		                    &position.x, &position.y, &quad, align_to_pixels);
		text2d->AddChar(CharacterVertex{{quad.x0, quad.y0}, {quad.s0, quad.t0}},
		                  CharacterVertex{{quad.x1, quad.y1}, {quad.s1, quad.t1}});
	}
	text2d->EndPlacing();
	// TODO: Make LRU cache
	text_cache_.insert_or_assign(std::move(text), text2d);
	return text2d;
}

bool Font::InitPackedAtlas() {
	data_->stb_packed_chars_.resize(consts::ascii_num + consts::cyrillic_num);
	auto ranges = std::to_array<stbtt_pack_range, 2>(
			{{// ascii
	      static_cast<float>(font_size_), consts::ascii_base, nullptr,
	      consts::ascii_num, data_->stb_packed_chars_.data()},
	     // cyrillic
	     {static_cast<float>(font_size_), consts::cyrillic_base, nullptr,
	      consts::cyrillic_num,
	      data_->stb_packed_chars_.data() + consts::ascii_num}});

	// TODO: Need to be unnailed, otherwise bigger texts will crash everything
	atlas_buffer_.resize(buffer_height_ * buffer_width_);
	{
		LOG_INFO("Packing font {}...", font_name_);
		stbtt_pack_context pack_ctx;
		if (!stbtt_PackBegin(&pack_ctx, atlas_buffer_.data(), buffer_width_,
		                     buffer_height_, 0, 1, nullptr)) {
			LOG_ERROR("Unable to initialize font packing");
			return false;
		}
		stbtt_PackSetOversampling(&pack_ctx, 1, 1);
		if (!stbtt_PackFontRanges(&pack_ctx, font_buffer_.data(), 0, ranges.data(),
		                          ranges.size())) {
			LOG_ERROR("Unable to pack font");
			return false;
		}
		stbtt_PackEnd(&pack_ctx);
		LOG_INFO("Packed!");
	}
	stbi_write_png("x1.png", buffer_width_, buffer_height_, 1, atlas_buffer_.data(), 0);
	return true;
}

}  // namespace renderer
