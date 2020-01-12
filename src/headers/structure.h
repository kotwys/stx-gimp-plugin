#pragma once
#include <cstdint>

#define STX_MAGIC_SIZE 3

#define STX_E1_BEGIN 225
#define STX_E1_SIZE 4

#define STX_GEOM_BEGIN 224
#define STX_GEOM_SIZE 28

#define STX_E6_BEGIN 230
#define STX_E6_SIZE 8

#define STX_DELIMITER 0

#define STX_NUM_CHANNELS 4

struct Geometry {
  uint16_t width;
  uint16_t height;
  uint16_t scale_x;
  uint16_t scale_y;
};
