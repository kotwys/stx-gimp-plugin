#pragma once

#include <gio/gio.h>
#include "stx/image.h"
#include "stx/value.h"

namespace stx {
  stx::Result<stx::Image> read(GInputStream *file);
}
