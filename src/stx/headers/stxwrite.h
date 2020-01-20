#pragma once
#include <variant>
#include <iostream>

#include "image.h"
#include "value.h"

namespace stx {
  StxResult<std::monostate> write(
    const StxImage &img,
    std::ostream &file
  );
}
