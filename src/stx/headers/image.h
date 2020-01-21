#pragma once
#include <cstdint>
#include <glib.h>

namespace stx {
  struct Geometry {
    uint16_t width;
    uint16_t height;
    uint16_t scale_x;
    uint16_t scale_y;
  };

  struct Image {
    Geometry geometry;
    guchar* image_data;
  };
}
