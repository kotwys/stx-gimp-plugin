#include "stx/bytes.h"
#include "stx/structure.h"
#include "stx/write.h"

static void g_output_stream_push(
  GOutputStream *stream,
  const char ch,
  GCancellable *cancellable,
  GError **error
) {
  char buffer[] = { ch };
  g_output_stream_write(stream, buffer, 1, cancellable, error);
}

#define CONVERT_SCALE(x) 25600 / x

static void build_geometry(
  unsigned char *buffer,
  const stx::Geometry &geometry,
  uint8_t magical_number
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

stx::Result<std::monostate> stx::write(
  const stx::Image &img,
  GOutputStream *file
) {
  using Result = stx::Result<std::monostate>;

  g_output_stream_write(file, STX_MAGIC, STX_MAGIC_SIZE, NULL, NULL);

  if (img.has_e6) {
    g_output_stream_push(file, STX_E6_BEGIN, NULL, NULL);
    g_output_stream_write(file, STX_E6, STX_E6_SIZE, NULL, NULL);
  }

  g_output_stream_push(file, STX_E1_BEGIN, NULL, NULL);
  g_output_stream_write(file, "\x20\x00", 2, NULL, NULL);
  g_output_stream_push(file, img.magical_number, NULL, NULL);
  g_output_stream_push(file, '\x00', NULL, NULL);

  g_output_stream_push(file, STX_GEOM_BEGIN, NULL, NULL);
  unsigned char geom_buffer[STX_GEOM_SIZE] = {0};

  build_geometry(geom_buffer, img.geometry, img.magical_number);
  g_output_stream_write(file, geom_buffer, STX_GEOM_SIZE, NULL, NULL);

  g_output_stream_write(file, "\x00\x00", 2, NULL, NULL);

  size_t bytes = img.geometry.width * img.geometry.height * STX_NUM_CHANNELS;
  g_output_stream_write(file, img.image_data, bytes, NULL, NULL);

  delete[] img.image_data;

  g_output_stream_write(file, "\x00\x00", 2, NULL, NULL);
  
  std::monostate unit;
  return OK(unit);
}
