#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <renderer/texture2d.h>

#include <glm/glm.hpp>

#include "text2d.h"
#include "font_material.h"

struct stbtt_fontinfo;
struct stbtt_pack_context;


namespace renderer {

class Font {
 public:
	using FontData = std::vector<unsigned char>;
	using FontAtlasBuffer = std::vector<unsigned char>;
	inline const static std::string default_font_name = "NotoSans-Regular";
	inline const static std::string font_extension = ".ttf";
	
	explicit Font(std::string font_name, int font_size = 24);
	~Font();

	bool Init();
	std::shared_ptr<Text2D> CreateText(std::u32string text, glm::vec2 position, bool align_to_pixels = false);
	FontMaterial& material() { return *material_; }

 private:
 	bool InitPackedAtlas();

	int font_size_;
	std::string font_name_;
	FontData font_buffer_;
	std::unique_ptr<stbtt_fontinfo> stb_font_ctx_;
	int buffer_width_ = 512;
	int buffer_height_ = 256;
	FontAtlasBuffer atlas_buffer_;
	std::shared_ptr<Texture2D> atlas_texture_;
	std::unordered_map<std::u32string, std::shared_ptr<Text2D>> text_cache_;
	std::unique_ptr<FontMaterial> material_;
	struct Impl;
	std::unique_ptr<Impl> data_;
	bool is_inited_ = false;
	bool has_errors_ = false;
};

}  // namespace renderer
