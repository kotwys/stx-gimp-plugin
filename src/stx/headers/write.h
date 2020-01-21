#pragma once
#include <variant>
#include <iostream>

#include "stx/image.h"
#include "stx/value.h"

namespace stx {
  stx::Result<std::monostate> write(
    const stx::Image &img,
    std::ostream &file
  );
}
