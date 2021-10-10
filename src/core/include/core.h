#pragma once

#include "IReader.h"

class Core {
public:
	static Core& instance() { static Core instance; return instance; }
private:
	Core() = default;
};
