#pragma once

#include <iostream>

#include "databases.h"
#include "idata_provider.h"
#include "shapes.h"


class IReader {
 public:
	// TODO we sould return new class FileRepresentation
	GeomStorage<BasicShape> geometry() { return data_provider_->geometry(); }
	MetadataStorage metadata() { return data_provider_->metadata(); }

 protected:
	virtual void load() = 0;
	std::unique_ptr<IDataProvider> data_provider_;
	virtual ~IReader() = default;
};
