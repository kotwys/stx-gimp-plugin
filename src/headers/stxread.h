#pragma once

#include <fstream>
#include "structure.h"
#include "value.h"

struct StxData {
  Geometry geometry;
  guchar* image_data;
};

StxResult<StxData> read_either(std::ifstream &file);
[[deprecated("Replaced by read_either")]]
StxError read(std::ifstream &file, StxData &data);

StxResult<gint32> to_image_either(const StxData &data);
[[deprecated("Replaced by to_image_either")]]
StxError to_image(const StxData &data, gint32 &image_id);
