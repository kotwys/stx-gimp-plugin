#include <algorithm>

#include "utils/bytes.h"
#include "stx/structure.h"
#include "stx/read.h"

static void parse_geometry(const unsigned char *buffer, stx::Geometry &geom) {
  geom.scale_x = read_l16(buffer + 5);
  geom.scale_y = read_l16(buffer + 9);
  geom.width = read_l16(buffer + 18);
  geom.height = read_l16(buffer + 26);
}

stx::Result<stx::Image> stx::read(Glib::RefPtr<Gio::InputStream> file) {
  using Result = stx::Result<stx::Image>;

  try {
    unsigned char magic[STX_MAGIC_SIZE];
    file->read(magic, STX_MAGIC_SIZE);

    if (!std::equal(magic, magic + STX_MAGIC_SIZE - 1, STX_MAGIC))
      return ERR(stx::Error::WRONG_TYPE);
  } catch (Glib::Error *err) {
    return ERR(stx::Error::EARLY_EOF);
  }

  stx::Image data;

  while (1) {
    try {
      unsigned char opener;
      file->read(&opener, 1);

      // TODO: Parse other sections
      if (opener == STX_E6_BEGIN) {
        file->skip(STX_E6_SIZE);
      } else if (opener == STX_E1_BEGIN) {
        file->skip(STX_E1_SIZE);
      } else if (opener == STX_GEOM_BEGIN) {
        auto buffer = new unsigned char[STX_GEOM_SIZE];
        file->read(buffer, STX_GEOM_SIZE);
        parse_geometry(buffer, data.geometry);
        delete[] buffer;
      } else if (opener == STX_DELIMITER) {
        break;
      }
    } catch (Glib::Error *err) {
      return ERR(stx::Error::EARLY_EOF);
    }
  }

  try {
    file->skip(1);

    size_t bytes =
      data.geometry.width *
      data.geometry.height *
      STX_NUM_CHANNELS;
    data.image_data = new unsigned char[bytes];
    file->read(data.image_data, bytes);
  } catch (Glib::Error *err) {
    if (data.image_data) {
      delete[] data.image_data;
    }

    return ERR(stx::Error::EARLY_EOF);
  }

  return OK(data);
}
