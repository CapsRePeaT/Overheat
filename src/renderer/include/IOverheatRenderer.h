#pragma once

class IOverheatRenderer {
public:
	virtual void initialize(void(* getProcAddressFunc)(const char*), int w = 0, int h = 0) = 0;
	virtual void render() = 0;
};
