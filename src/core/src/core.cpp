#include "core.h"

#include <iostream>

#include "../../readers/src/solver3d/solver3d_reader.h"
#include "../../readers/src/solver2d/solver2d_reader.h"
#include "databases.h"

NameAndIds Core::GetRepresentationsData() const { 
	NameAndIds result; 
	for (const auto& representation : representations_)
		result.emplace_back(std::move(representation.GetName(representation.id())),
		                    representation.id());
	return result;

}

NameAndIds Core::GetLayersData(GlobalId representation_id) const {
	assert(representation_id.type() == InstanceType::Representation);
	const auto& representation =
			GetRepresentation(representation_id.representation_id());
	NameAndIds result;
	for (const auto& id : representation.GetAllLayerIds())
		result.emplace_back(std::move(representation.GetName(id)), id);
	return result;
}

NameAndIds Core::GetShapesData(GlobalId layer_id) const { 
	assert(layer_id.type() == InstanceType::Layer);
	const auto& representation = GetRepresentation(layer_id.representation_id());
	NameAndIds result;
	for (const auto& id : representation.GetAllShapeIdsOfLayer(layer_id))
		result.emplace_back(std::move(representation.GetName(id)), id);
	return result;
}

void Core::LoadGeometry(std::string trm_file_path_mv,
                        std::string t2d_file_path_mv, 
	                      const GeometryType type) {
	switch (type) {
		case GeometryType::D3: {
		  Readers::Solver3d::Solver3dReader reader_3d(std::move(trm_file_path_mv),
	                                                std::move(t2d_file_path_mv));
			representations_.emplace_back(reader_3d.geometry(), reader_3d.heatmaps());
			break;
		}
		case GeometryType::D2: {
			Readers::Solver2d::Solver2dReader reader_2d(std::move(trm_file_path_mv),
	                                                std::move(t2d_file_path_mv));
			representations_.emplace_back(reader_2d.geometry(), reader_2d.heatmaps());
			break;
		}
		default:
			assert(false && "undefined geom type");
	}
	
	// TODO: If we create new file representation here,
	// how can reader set its id to shapes?
}
