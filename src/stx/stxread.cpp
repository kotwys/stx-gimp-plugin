#include <algorithm>
#include <sstream>
#include <libgimp/gimp.h>

#include "bytes.h"
#include "structure.h"
#include "stxread.h"

using stream = std::istream;

static StxError parse_geometry(const char *buffer, Geometry &geom) {
  geom.scale_x = read_l16(buffer + 5);
  geom.scale_y = read_l16(buffer + 9);
  geom.width = read_l16(buffer + 18);
  geom.height = read_l16(buffer + 26);
  return StxError::SUCCESS;
}

StxResult<StxImage> stx::read(stream &file) {
  using Result = StxResult<StxImage>;

  char magic[STX_MAGIC_SIZE];
  file.read(magic, STX_MAGIC_SIZE);
  // Check signature
  if (!std::equal(magic, magic + STX_MAGIC_SIZE - 1, STX_MAGIC))
    return ERR(StxError::WRONG_TYPE);

  if (!file.good())
    return ERR(StxError::EARLY_EOF);

  StxImage data;

  while (1) {
    auto opener = file.get();
    if (!file.good())
      return ERR(StxError::EARLY_EOF);

    // TODO: Parse other sections
    if (opener == STX_E6_BEGIN) {
      file.seekg(STX_E6_SIZE, stream::cur);
    } else if (opener == STX_E1_BEGIN) {
      file.seekg(STX_E1_SIZE, stream::cur);
    } else if (opener == STX_GEOM_BEGIN) {
      char *buffer = new char[STX_GEOM_SIZE];
      file.read(buffer, STX_GEOM_SIZE);
      if (!file.good()) {
        delete[] buffer;
        return ERR(StxError::EARLY_EOF);
      }

      StxError err = parse_geometry(buffer, data.geometry);
      delete[] buffer;
      if (err != StxError::SUCCESS) {
        return ERR(err);
      }
    } else if (opener == STX_DELIMITER) {
      break;
    }
  }

  file.seekg(1, stream::cur);
  if (!file.good())
    return ERR(StxError::EARLY_EOF);

  size_t pixels = data.geometry.width * data.geometry.height;
  data.image_data = new guchar[pixels * STX_NUM_CHANNELS];

  char buffer[STX_NUM_CHANNELS];
  for (size_t i = 0; i < pixels; i++) {
    size_t pos = i * STX_NUM_CHANNELS;
    file.read(buffer, STX_NUM_CHANNELS);
    if (!file.good()) {
      return ERR(StxError::EARLY_EOF);
    }

    // RGBA <- BGRA
    data.image_data[pos] = buffer[2];
    data.image_data[pos + 1] = buffer[1];
    data.image_data[pos + 2] = buffer[0];
    data.image_data[pos + 3] = buffer[3];
  }

  if (!file.good())
    return ERR(StxError::EARLY_EOF);

  return OK(data);
}
