#pragma once

#include <fstream>
#include "image.h"
#include "value.h"

StxResult<StxImage> read(std::ifstream &file);
StxResult<gint32> to_image(const StxImage &data);
