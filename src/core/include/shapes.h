#pragma once

#include <memory>
#include <vector>

#include "common.h"



enum class HilightSetup {
	none = 0,
	basic, // just black thin line
	stack_hilight, // when selected in side widget or clicked on stack
	main_hilight // for this shape we show metadata 
};

class VisualisationOptions {
	bool is_visible() { return is_visible_; }
	void set_visibility(bool is_visible) { is_visible_ = is_visible; }
	HilightSetup hilight() { return hilight_; }
	void set_hilight(HilightSetup hilight) { hilight_ = hilight; }
 private:
	bool is_visible_ = true;
	HilightSetup hilight_ = HilightSetup::basic;
};

using GlobalShapeIds = std::vector<GlobalId>;

class BasicShape {
 public:
	explicit BasicShape(const GlobalId id) : id_(id) {}
	BasicShape(const GlobalId id, const LayerId layer_id, Box3D bbox_mv)
			: id_(id), layer_id_(layer_id), bbox_(std::move(bbox_mv)) {}
	[[nodiscard]] GlobalId id() const { return id_; }
	[[nodiscard]] const Box3D& bbox() const { return bbox_; }
	void setBox(Box3D box_mv) { bbox_ = std::move(box_mv); }
	VisualisationOptions& visualisation_options() {
		return visualisation_options_;
	}
	[[nodiscard]] LayerId layer_id() const { return layer_id_; }
 private:
	const GlobalId id_;
	const LayerId layer_id_ = UndefinedId;
	ShapeType shape_type_   = ShapeType::Undefined;
	Box3D bbox_;
	VisualisationOptions visualisation_options_;
};

using ShapePtr = std::shared_ptr<BasicShape>;
using Shapes   = std::vector<ShapePtr>;
