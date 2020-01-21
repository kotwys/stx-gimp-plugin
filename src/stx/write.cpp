#include <algorithm>
#include <libgimp/gimp.h>

#include "stx/bytes.h"
#include "stx/structure.h"
#include "stx/write.h"

using stream = std::ofstream;

#define CONVERT_SCALE(x) 25600 / x

static void build_geometry(
  char *buffer,
  const stx::Geometry &geometry,
  guint8 magical_number
) {
  buffer[0] = 0x1a;
  buffer[2] = magical_number;
  buffer[4] = 0x01;
  write_l16(buffer + 5, CONVERT_SCALE(geometry.scale_x));
  write_l16(buffer + 9, CONVERT_SCALE(geometry.scale_y));
  buffer[17] = 0x01;
  write_l16(buffer + 18, geometry.width);
  write_l16(buffer + 22, geometry.width * 4);
  write_l16(buffer + 26, geometry.height);
}

#define STX_E6 "\x04\x00\x00\x00\x06\x00\x00\x00"

#define DEFAULT_NUMBER '\x04'

stx::Result<std::monostate> stx::write(
  const stx::Image &img,
  std::ostream &file
) {
  using Result = stx::Result<std::monostate>;

  file.write(STX_MAGIC, STX_MAGIC_SIZE);

  /*
  if (params.e6_write)
    file.put(STX_E6_BEGIN)
      .write(STX_E6, STX_E6_SIZE);
  */

  file.put(STX_E1_BEGIN);
  file.write("\x20\x00", 2)
    .put(DEFAULT_NUMBER)
    .put('\x00');

  file.put(STX_GEOM_BEGIN);
  char geom_buffer[STX_GEOM_SIZE] = {0};

  build_geometry(geom_buffer, img.geometry, DEFAULT_NUMBER);
  file.write(geom_buffer, STX_GEOM_SIZE);

  file.write("\x00\x00", 2);

  size_t bytes = img.geometry.width * img.geometry.height * STX_NUM_CHANNELS;
  file.write(img.image_data, bytes);

  delete[] img.image_data;

  file.write("\x00\x00", 2);
  
  if (!file.good())
    return ERR(stx::Error::WRITTING_ERROR);

  std::monostate unit;
  return OK(unit);
}
