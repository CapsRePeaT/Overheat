#pragma once

#include "common.h"

class IRenderer {
 public:
  virtual void SetDrawMode(const DrawMode) = 0;
  virtual void initialize(void(* getProcAddressFunc)(const char*), int w = 0, int h = 0) = 0;
 protected:
  virtual void render() = 0;
 private:
};
