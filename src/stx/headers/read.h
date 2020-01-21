#pragma once

#include <iostream>
#include "stx/image.h"
#include "stx/value.h"

namespace stx {
  stx::Result<stx::Image> read(std::istream &file);
}
