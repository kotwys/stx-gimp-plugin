#pragma once
#include <variant>
#include <gio/gio.h>

#include "stx/image.h"
#include "stx/value.h"

namespace stx {
  stx::Result<std::monostate> write(
    const stx::Image &img,
    GOutputStream *file
  );
}
