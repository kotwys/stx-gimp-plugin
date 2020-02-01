#include <algorithm>
#include <sstream>
#include <libgimp/gimp.h>

#include "stx/bytes.h"
#include "stx/structure.h"
#include "stx/read.h"

static void parse_geometry(const char *buffer, stx::Geometry &geom) {
  geom.scale_x = read_l16(buffer + 5);
  geom.scale_y = read_l16(buffer + 9);
  geom.width = read_l16(buffer + 18);
  geom.height = read_l16(buffer + 26);
}

stx::Result<stx::Image> stx::read(GInputStream *file) {
  using Result = stx::Result<stx::Image>;
  g_autoptr(GError) err = NULL;

  unsigned char magic[STX_MAGIC_SIZE];
  g_input_stream_read(file, magic, STX_MAGIC_SIZE, NULL, &err);
  if (err != NULL)
    return ERR(stx::Error::EARLY_EOF);

  // Check signature
  if (!std::equal(magic, magic + STX_MAGIC_SIZE - 1, STX_MAGIC))
    return ERR(stx::Error::WRONG_TYPE);

  stx::Image data;

  while (1) {
    unsigned char opener;
    g_input_stream_read(file, &opener, 1, NULL, &err);
    if (err != NULL)
      return ERR(stx::Error::EARLY_EOF);

    // TODO: Parse other sections
    if (opener == STX_E6_BEGIN) {
      g_input_stream_skip(file, STX_E6_SIZE, NULL, NULL);
    } else if (opener == STX_E1_BEGIN) {
      g_input_stream_skip(file, STX_E1_SIZE, NULL, NULL);
    } else if (opener == STX_GEOM_BEGIN) {
      auto buffer = new unsigned char[STX_GEOM_SIZE];
      g_input_stream_read(file, buffer, STX_GEOM_SIZE, NULL, &err);
      if (err != NULL) {
        delete[] buffer;
        return ERR(stx::Error::EARLY_EOF);
      }

      parse_geometry(buffer, data.geometry);
      delete[] buffer;
    } else if (opener == STX_DELIMITER) {
      break;
    }
  }

  g_input_stream_skip(file, 1, NULL, &err);
  if (err != NULL)
    return ERR(stx::Error::EARLY_EOF);

  size_t bytes =
    data.geometry.width *
    data.geometry.height *
    STX_NUM_CHANNELS;
  data.image_data = new unsigned char[bytes];
  g_input_stream_read(file, data.image_data, bytes, NULL, &err);

  if (err != NULL) {
    delete[] data.image_data;
    return ERR(stx::Error::EARLY_EOF);
  }

  return OK(data);
}
