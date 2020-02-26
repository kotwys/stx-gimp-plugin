#include "stx/bytes.h"
#include "stx/structure.h"
#include "stx/write.h"

static void g_output_stream_push(
  Glib::RefPtr<Gio::OutputStream> stream,
  const char ch
) {
  char buffer[] = { ch };
  stream->write(buffer, 1);
}

#define CONVERT_SCALE(x) 25600 / x

static void build_geometry(
  unsigned char *buffer,
  const stx::Geometry &geometry,
  uint16_t magical_number
) {
  buffer[0] = 0x1a;
  write_l16(buffer + 1, magical_number);
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
  Glib::RefPtr<Gio::OutputStream> file
) {
  using Result = stx::Result<std::monostate>;

  try {
    file->write(STX_MAGIC, STX_MAGIC_SIZE);

    if (img.has_e6) {
      g_output_stream_push(file, STX_E6_BEGIN);
      file->write(STX_E6, STX_E6_SIZE);
    }

    g_output_stream_push(file, STX_E1_BEGIN);
    g_output_stream_push(file, '\x20');

    {
      unsigned char magic_buffer[2];
      write_l16(magic_buffer, img.magical_number);
      file->write(magic_buffer, 2);
    }
    g_output_stream_push(file, '\x00');

    g_output_stream_push(file, STX_GEOM_BEGIN);
    unsigned char geom_buffer[STX_GEOM_SIZE] = {0};

    build_geometry(geom_buffer, img.geometry, img.magical_number);
    file->write(geom_buffer, STX_GEOM_SIZE);

    file->write("\x00\x00", 2);

    size_t bytes =
      img.geometry.width *
      img.geometry.height *
      STX_NUM_CHANNELS;
    file->write(img.image_data, bytes);

    delete[] img.image_data;

    file->write("\x00\x00", 2);
  } catch (Glib::Error err) {
    delete[] img.image_data;
    return ERR(stx::Error::WRITTING_ERROR);
  }
  
  std::monostate unit;
  return OK(unit);
}
