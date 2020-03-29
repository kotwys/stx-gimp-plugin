#pragma once
#include "stx/geom/decoder.h"

namespace stx::geom {
  class PCGeomDecoder : public AbstractGeomDecoder {
  public:
    void decode(const unsigned char *buf, Geometry &geom);
    size_t required_length();
  };
}
