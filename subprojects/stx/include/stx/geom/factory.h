#pragma once
#include <memory>
#include "stx/geom/decoder.h"

namespace stx::geom {
  std::unique_ptr<AbstractGeomDecoder> get_decoder(unsigned char code);
}
