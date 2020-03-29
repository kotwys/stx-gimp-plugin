#include "stx/geom/pc_decoder.h"
#include "utils/bytes.h"

namespace stx::geom {
  size_t PCGeomDecoder::required_length() {
    return 25;
  }

  void PCGeomDecoder::decode(const unsigned char *buf, Geometry &geom) {
    geom.scale_x = read_l16(buf + 2);
    geom.scale_y = read_l16(buf + 6);
    geom.width = read_l16(buf + 15);
    geom.height = read_l16(buf + 23);
  }
}
