#include "core.h"

#include <iostream>

#include "../../readers/src/solver3d/solver3d_reader.h"
#include "../../readers/src/solver2d/solver2d_reader.h"

NameAndIds Core::GetRepresentationsData() const { 
	NameAndIds result; 
	for (const auto& representation : representations_)
		result.push_back({std::move(representation.GetName(representation.id())),
		                  representation.id()});
	return result;

}

NameAndIds Core::GetLayersData(GlobalId representation_id) const {
	assert(representation_id.type() == InstanceType::Representation);
	const auto& representation =
			GetRepresentation(representation_id.representation_id());
	NameAndIds result;
	for (const auto& id : representation.GetAllLayerIds())
		result.push_back({representation.GetName(id), id});
	return result;
}

NameAndIds Core::GetShapesData(GlobalId layer_id) const { 
	assert(layer_id.type() == InstanceType::Layer);
	const auto& representation = GetRepresentation(layer_id.representation_id());
	NameAndIds result;
	for (const auto& id : representation.GetAllShapeIdsOfLayer(layer_id))
		result.push_back({representation.GetName(id), id});
	return result;
}

void Core::LoadGeometry(std::string trm_file_path_mv,
                        std::string t2d_file_path_mv, 
	                      const GeometryType type) {
	// TODO make proper interface class usage
	FileRepresentation new_representation;
	auto FillRepresentation = [&new_representation](auto& reader) {
		new_representation.geom_storage() = reader.geometry();
		new_representation.heatmaps()     = reader.heatmaps();
	};
	switch (type) {
		case GeometryType::D3: {
		  Readers::Solver3d::Solver3dReader reader_3d(std::move(trm_file_path_mv),
	                                                std::move(t2d_file_path_mv));
			FillRepresentation(reader_3d);
			break;
		}
		case GeometryType::D2: {
			Readers::Solver2d::Solver2dReader reader_2d(std::move(trm_file_path_mv),
	                                                std::move(t2d_file_path_mv));
			FillRepresentation(reader_2d);
			break;
		}
		default:
			assert(false && "undefined geom type");
	}
	
	// TODO: If we create new file representation here,
	// how can reader set its id to shapes?
	// TODO: BTW, we assign objects to getters here.
	//  1. We need setter for this operation
	//  2. Copy of 2 huge objects. Should be using pointers/references 
	//     or anything else to avoid it
	representations_.emplace_back(std::move(new_representation));
}
