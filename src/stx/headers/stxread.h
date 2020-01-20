#pragma once

#include <iostream>
#include "image.h"
#include "value.h"

namespace stx {
  StxResult<StxImage> read(std::istream &file);
}
