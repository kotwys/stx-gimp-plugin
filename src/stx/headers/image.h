#pragma once
#include <cstdint>
#include <glib.h>

struct Geometry {
  uint16_t width;
  uint16_t height;
  uint16_t scale_x;
  uint16_t scale_y;
};

struct StxImage {
  Geometry geometry;
  guchar* image_data;
};
