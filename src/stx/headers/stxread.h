#pragma once

#include <iostream>
#include "image.h"
#include "value.h"

StxResult<StxImage> read(std::istream &file);
StxResult<gint32> to_image(const StxImage &data);
