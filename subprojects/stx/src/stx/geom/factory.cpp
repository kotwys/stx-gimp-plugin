#include "stx/structure.h"
#include "stx/geom/factory.h"
#include "stx/geom/pc_decoder.h"

namespace stx::geom {
  std::unique_ptr<AbstractGeomDecoder> get_decoder(unsigned char code) {
    std::unique_ptr<AbstractGeomDecoder> ptr;
    switch (code) {
      case STX_GEOM_PC:
        ptr = std::make_unique<PCGeomDecoder>();
        break;
    }
    return ptr;
  }
}
