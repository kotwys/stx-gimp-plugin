#include <babl/babl.h>

#include "gimp_interop.h"
#include "stx/structure.h"

stx::Result<gint32> to_gimp(const stx::Image &data) {
  using Result = stx::Result<gint32>;

  gint32 image_id = gimp_image_new(
    data.geometry.width,
    data.geometry.height,
    GIMP_RGB
  );

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
    return ERR(stx::Error::GIMP_ERROR);
  }

  babl_init();

  GeglBuffer *buffer = gimp_drawable_get_buffer(layer_id);
  const Babl *bgra = babl_format_new(
    babl_model("R~G~B~A"),
    babl_type("u8"),
    babl_component("B~"),
    babl_component("G~"),
    babl_component("R~"),
    babl_component("A"),
    NULL
  );

  gegl_buffer_set(
    buffer,
    GEGL_RECTANGLE(0, 0, data.geometry.width, data.geometry.height),
    0, bgra, data.image_data, GEGL_AUTO_ROWSTRIDE
  );

  delete[] data.image_data;

  gegl_buffer_flush(buffer);
  g_object_unref(buffer);

  babl_exit();

  return OK(image_id);
}

stx::Result<stx::Image> from_gimp(
  const StxParams &params,
  const gint32 drawable_id
) {
  using Result = stx::Result<stx::Image>;

  stx::Image img;
  img.has_e6 = params.e6_write;
  img.magical_number = params.magical_number;

  GeglBuffer *buffer = gimp_drawable_get_buffer(drawable_id);

  guint16 width = gegl_buffer_get_width(buffer);
  guint16 height = gegl_buffer_get_height(buffer);

  stx::Geometry geometry = {
    width,
    height,
    params.scale_x,
    params.scale_y
  };
  img.geometry = geometry;

  babl_init();

  img.image_data = new unsigned char[width * height * STX_NUM_CHANNELS];
  const Babl *bgra = babl_format_new(
    babl_model("R~G~B~A"),
    babl_type("u8"),
    babl_component("B~"),
    babl_component("G~"),
    babl_component("R~"),
    babl_component("A"),
    NULL
  );
  gegl_buffer_get(
    buffer,
    GEGL_RECTANGLE(0, 0, width, height),
    1.0, bgra, img.image_data, GEGL_AUTO_ROWSTRIDE,
    GEGL_ABYSS_NONE
  );

  g_object_unref(buffer);
  babl_exit();

  return OK(img);
}
