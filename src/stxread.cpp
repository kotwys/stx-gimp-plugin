#include <fstream>
#include <sstream>
#include <libgimp/gimp.h>

#include "bytes.h"
#include "structure.h"
#include "stxread.h"
#include "value.h"

using stream = std::ifstream;

static StxError parse_geometry(const char *buffer, Geometry &geom) {
  geom.scale_x = read_l16(buffer + 5);
  geom.scale_y = read_l16(buffer + 9);
  geom.width = read_l16(buffer + 18);
  geom.height = read_l16(buffer + 26);
  return StxError::SUCCESS;
}

StxError read(stream &file, StxData &data) {
  file.seekg(STX_MAGIC_SIZE);
  if (!file.good())
    return StxError::EARLY_EOF;

  while (1) {
    auto opener = file.get();
    if (!file.good())
      return StxError::EARLY_EOF;

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
        return StxError::EARLY_EOF;
      }

      StxError err = parse_geometry(buffer, data.geometry);
      delete[] buffer;
      if (err != StxError::SUCCESS) {
        return err;
      }
    } else if (opener == STX_DELIMITER) {
      break;
    }
  }

  file.seekg(1, stream::cur);
  if (!file.good())
    return StxError::EARLY_EOF;

  size_t pixels = data.geometry.width * data.geometry.height;
  data.image_data = new guchar[pixels * STX_NUM_CHANNELS];

  char buffer[STX_NUM_CHANNELS];
  for (size_t i = 0; i < pixels; i++) {
    size_t pos = i * STX_NUM_CHANNELS;
    file.read(buffer, STX_NUM_CHANNELS);
    if (!file.good()) {
      return StxError::EARLY_EOF;
    }

    // RGBA <- BGRA
    data.image_data[pos] = buffer[2];
    data.image_data[pos + 1] = buffer[1];
    data.image_data[pos + 2] = buffer[0];
    data.image_data[pos + 3] = buffer[3];
  }

  if (!file.good())
    return StxError::EARLY_EOF;

  file.close();

  return StxError::SUCCESS;
}

StxError to_image(const StxData &data, gint32 &image_id) {
  image_id = gimp_image_new(
    data.geometry.width,
    data.geometry.height,
    GIMP_RGB
  );
  //gimp_image_set_filename(image_id, filename);

  gint32 layer_id = gimp_layer_new(
    image_id,
    "Texture",
    data.geometry.width, data.geometry.height,
    GIMP_RGBA_IMAGE,
    100.0,
    GIMP_NORMAL_MODE
  );
  gboolean success = gimp_image_insert_layer(image_id, layer_id, 0, 0);
  
  if (!success) {
    gimp_image_delete(image_id);
    return StxError::GIMP_ERROR;
  }

  GimpDrawable *drawable = gimp_drawable_get(layer_id);

  GimpPixelRgn rgn_out;
  gimp_pixel_rgn_init(
    &rgn_out, drawable,
    0, 0, data.geometry.width, data.geometry.height,
    TRUE, TRUE
  );

  gimp_pixel_rgn_set_rect(
    &rgn_out, data.image_data,
    0, 0,
    data.geometry.width, data.geometry.height
  );

  delete[] data.image_data;

  gimp_drawable_flush(drawable);
  gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
  gimp_drawable_update(
    drawable->drawable_id,
    0, 0,
    data.geometry.width, data.geometry.height
  );

  gimp_drawable_detach(drawable);

  return StxError::SUCCESS;
}
