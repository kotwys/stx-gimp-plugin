#include <babl/babl.h>

#include "gimp_interop.h"
#include "stx/structure.h"

static long convert_rgba(const uint8_t *src, uint8_t *dst, long pixels) {
  const Babl *bgra = babl_format_new(
    babl_model("RGBA"),
    babl_type("u8"),
    babl_component("B"),
    babl_component("G"),
    babl_component("R"),
    babl_component("A"),
    NULL
  );

  const Babl *rgba = babl_format("RGBA");
  const Babl *bgra_to_rgba = babl_fish(bgra, rgba);

  babl_init();

  return babl_process(bgra_to_rgba, src, dst, pixels);
}

static long convert_bgra(const uint8_t *src, uint8_t *dst, long pixels) {
  const Babl *bgra = babl_format_new(
    babl_model("RGBA"),
    babl_type("u8"),
    babl_component("B"),
    babl_component("G"),
    babl_component("R"),
    babl_component("A"),
    NULL
  );

  const Babl *rgba = babl_format("RGBA");
  const Babl *rgba_to_bgra = babl_fish(rgba, bgra);

  babl_init();

  return babl_process(rgba_to_bgra, src, dst, pixels);
}

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

  GimpDrawable *drawable = gimp_drawable_get(layer_id);

  GimpPixelRgn rgn_out;
  gimp_pixel_rgn_init(
    &rgn_out, drawable,
    0, 0, data.geometry.width, data.geometry.height,
    TRUE, TRUE
  );

  long pixels = data.geometry.width * data.geometry.height;
  uint8_t *tmp = new uint8_t[pixels * 4];
  convert_rgba(data.image_data, tmp, pixels);

  delete[] data.image_data;

  gimp_pixel_rgn_set_rect(
    &rgn_out, tmp,
    0, 0,
    data.geometry.width, data.geometry.height
  );

  delete[] tmp;

  gimp_drawable_flush(drawable);
  gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
  gimp_drawable_update(
    drawable->drawable_id,
    0, 0,
    data.geometry.width, data.geometry.height
  );

  gimp_drawable_detach(drawable);

  return OK(image_id);
}

stx::Result<stx::Image> from_gimp(
  const StxParams &params,
  const gint32 drawable_id
) {
  using Result = stx::Result<stx::Image>;

  stx::Image img;

  guint16 width = gimp_drawable_width(drawable_id);
  guint16 height = gimp_drawable_height(drawable_id);

  stx::Geometry geometry = {
    width,
    height,
    params.scale_x,
    params.scale_y
  };
  img.geometry = geometry;

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
  convert_bgra(data, img.image_data, pixels);

  delete[] data;

  return OK(img);
}
