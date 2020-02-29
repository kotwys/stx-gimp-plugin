#pragma once

#include <giomm.h>
#include "stx/image.h"
#include "stx/value.h"

namespace stx {
  stx::Result<stx::Image> read(Glib::RefPtr<Gio::InputStream> file);
}
