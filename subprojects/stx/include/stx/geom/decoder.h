#pragma once
#include <cstddef>
#include "stx/image.h"

namespace stx::geom {
  class AbstractGeomDecoder {
  public:
    virtual void decode(const unsigned char *buf, Geometry &geom) =0;
    virtual size_t required_length() =0;
  };
}
