#pragma once
#include <variant>
#include <giomm.h>

#include "stx/image.h"
#include "stx/value.h"

namespace stx {
  stx::Result<std::monostate> write(
    const stx::Image &img,
    Glib::RefPtr<Gio::OutputStream> file
  );
}
