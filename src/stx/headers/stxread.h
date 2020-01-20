#pragma once

#include <fstream>
#include "structure.h"
#include "value.h"

struct StxData {
  Geometry geometry;
  guchar* image_data;
};

StxResult<StxData> read(std::ifstream &file);
StxResult<gint32> to_image(const StxData &data);
