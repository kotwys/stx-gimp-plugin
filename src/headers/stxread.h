#pragma once

#include <fstream>
#include "structure.h"
#include "value.h"

struct StxData {
  Geometry geometry;
  guchar* image_data;
};

StxError read(std::ifstream &file, StxData &data);
StxError to_image(const StxData &data, gint32 &image_id);
