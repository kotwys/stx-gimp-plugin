#include <algorithm>
#include <variant>
#include <glib.h>
#include <libgimp/gimp.h>

#include "bytes.h"
#include "structure.h"
#include "image.h"
#include "stxwrite.h"
#include "value.h"

using stream = std::ofstream;

StxParams default_params() {
  StxParams params;
  params.scale_x = 100;
  params.scale_y = 100;
  params.e6_write = FALSE;
  params.magical_number = 4;
  return params;
}

#define CONVERT_SCALE(x) 25600 / x

static void build_geometry(
  char *buffer,
  const Geometry &geometry,
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

StxResult<std::monostate> write(
  const StxImage &img,
  std::ofstream &file
) {
  using Result = StxResult<std::monostate>;

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
    return ERR(StxError::WRITTING_ERROR);

  std::monostate unit;
  return OK(unit);
}

StxResult<std::monostate> write(
  const StxParams &params,
  gint32 drawable_id,
  std::ofstream &file
) {
  using Result = StxResult<std::monostate>;

  StxImage img;

  guint16 width = gimp_drawable_width(drawable_id);
  guint16 height = gimp_drawable_height(drawable_id);

  img.geometry = {
    width,
    height,
    params.scale_x,
    params.scale_y
  };

  const bool has_alpha = gimp_drawable_has_alpha(drawable_id);
  const size_t offset = has_alpha ? 4 : 3;

  GimpPixelRgn rgn_in;
  GimpDrawable *drawable = gimp_drawable_get(drawable_id);

  gimp_pixel_rgn_init(&rgn_in, drawable, 0, 0, width, height, FALSE, FALSE);
  size_t pixels = width * height;
  guchar* data = new guchar[pixels * offset];
  gimp_pixel_rgn_get_rect(&rgn_in, data, 0, 0, width, height);
  gimp_drawable_detach(drawable);

  img.image_data = new guchar[pixels * STX_NUM_CHANNELS];

  for (size_t i = 0; i < pixels; i++) {
    size_t src_pos = i * offset;
    size_t dst_pos = i * STX_NUM_CHANNELS;
    // RGBA -> BGRA
    img.image_data[dst_pos] = data[src_pos + 2];
    img.image_data[dst_pos + 1] = data[src_pos + 1];
    img.image_data[dst_pos + 2] = data[src_pos];
    img.image_data[dst_pos + 3] = has_alpha ? data[src_pos + 3] : 0xff;
  }

  delete[] data;

  return write(img, file);
}
